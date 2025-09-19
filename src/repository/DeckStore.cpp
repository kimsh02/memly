#include "repository/DeckStore.hpp"

#include <expected>

#include "common/Types.hpp"
#include "util/Util.hpp"

[[nodiscard]] Types::IDVector DeckStore::GetDeckIDs() const {
    Types::IDVector idv;
    idv.reserve(m_DeckRecords.size());
    for (auto it = m_DeckRecords.begin(); it != m_DeckRecords.end(); it++) {
        idv.push_back(it->second.DeckContext.ID());
    }
    return idv;
}

DeckRecord DeckStore::dbRead(std::size_t deckID) {
    m_ReadStmt.Bind(deckID);
    m_ReadStmt.Exec();
    if (!m_ReadStmt.Next()) {
        Fatal("ReadStmt.Exec() did not return a row");
    }
    DeckContext deckContext(m_ReadStmt.Value(0).toULongLong());
    if (auto res = validateContextFields(deckContext); !res) {
        Fatal(Types::VResultToString(res));
    }
    DeckStats deckStats(m_ReadStmt.Value(1).toULongLong());
    if (auto res = validateStatFields(deckStats); !res) {
        Fatal(Types::VResultToString(res));
    }
    DeckInfo deckInfo(m_ReadStmt.Value(2).toString().toStdString());
    m_ReadStmt.Finish();
    if (auto res = validateInfoFields(deckInfo); !res) {
        Fatal(Types::VResultToString(res));
    }
    Deck deck(std::move(deckInfo));
    return DeckRecord(std::move(deckContext), std::move(deckStats), std::move(deck));
}

DeckStore::DeckStore(const DatabaseQt& db) noexcept
    : m_Db(db) {
    auto getIDs = db.Prepare(SQL::s_GetIDsSQL);
    getIDs.Exec();
    while (getIDs.Next()) {
        std::size_t deckID = getIDs.Value(0).toULongLong();
        m_DeckRecords.emplace(deckID, dbRead(deckID));
    }
}

[[nodiscard]] DeckStore::CVResult
DeckStore::validateContextFields(const DeckContext& deckContext) const {
    Types::ValidationErrors<ContextField> v;
    if (auto id = deckContext.ID() < 0) {
        v.emplace_back(ContextField::ID, std::format("Invalid deck id: {}", id));
    }
    if (v.empty())
        return CVResult{};
    return std::unexpected(std::move(v));
}

[[nodiscard]] DeckStore::SVResult DeckStore::validateStatFields(const DeckStats& deckStats) const {
    Types::ValidationErrors<StatField> v;
    if (auto cc = deckStats.CardCount() < 0) {
        v.emplace_back(StatField::CardCount, std::format("Invalid deck card count: {}", cc));
    }
    if (v.empty())
        return SVResult{};
    return std::unexpected(std::move(v));
}

[[nodiscard]] DeckStore::IVResult DeckStore::validateInfoFields(const DeckInfo& deckInfo) const {
    Types::ValidationErrors<InfoField> v;
    if (const auto& s = deckInfo.Name; s.size() > Limit::s_MAX_NAME_LEN) {
        v.emplace_back(
            InfoField::Name,
            std::format("Deck name exceeds {} characters: {}", Limit::s_MAX_NAME_LEN, s));
    }
    if (v.empty())
        return IVResult{};
    return std::unexpected(std::move(v));
}

[[nodiscard]] DeckStore::IVResult DeckStore::Create(Deck&& deck) {
    if (auto res = validateInfoFields(deck.DeckInfo); !res) {
        return res;
    }
    m_CreateStmt.Bind(deck.DeckInfo.Name);
    m_CreateStmt.ExecImmediate();
    std::size_t deckID = m_CreateStmt.LastInsertID();
    m_CreateStmt.Finish();

    m_DeckRecords.emplace(deckID, dbRead(deckID));
    return IVResult{};
}

[[nodiscard]] const Deck* DeckStore::Read(std::size_t deckID) noexcept {
    return &getDeckRecordMapIter(deckID)->second.Deck;
}

[[nodiscard]] DeckStore::IVResult DeckStore::Update(std::size_t deckID, Deck&& deck) {
    auto it = getDeckRecordMapIter(deckID);

    if (auto res = validateInfoFields(deck.DeckInfo); !res) {
        return res;
    }
    m_UpdateStmt.Bind(deck.DeckInfo.Name);
    m_UpdateStmt.ExecImmediate();
    m_UpdateStmt.Finish();

    it->second = dbRead(deckID);
    return IVResult{};
}

void DeckStore::Delete(std::size_t deckID) noexcept {
    auto it = getDeckRecordMapIter(deckID);

    m_DeleteStmt.Bind(deckID);
    m_DeleteStmt.ExecImmediate();
    m_DeleteStmt.Finish();

    m_DeckRecords.erase(it);
}

[[nodiscard]] DeckStore::DeckRecordMap::iterator
DeckStore::getDeckRecordMapIter(std::size_t deckID) noexcept {
    auto it = m_DeckRecords.find(deckID);
    if (it == m_DeckRecords.end()) {
        Fatal(std::format("Invalid deckID: {}", deckID));
    }
    return it;
}
