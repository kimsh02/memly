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

DeckRecord DeckStore::dbRead(DatabaseQt::Stmt& stmt, bool single) {
    DeckContext deckContext(stmt.Value(0).toULongLong());
    if (auto res = validateContextFields(deckContext); !res) {
        Fatal(Types::VResultToString(res));
    }
    DeckStats deckStats(stmt.Value(1).toULongLong());
    if (auto res = validateStatFields(deckStats); !res) {
        Fatal(Types::VResultToString(res));
    }
    DeckInfo deckInfo(stmt.Value(2).toString().toStdString());
    if (single)
        stmt.Finish();
    if (auto res = validateInfoFields(deckInfo); !res) {
        Fatal(Types::VResultToString(res));
    }
    Deck deck(std::move(deckStats), std::move(deckInfo));
    return DeckRecord(std::move(deckContext), std::move(deck));
}

DeckStore::DeckStore(const DatabaseQt& db, CardStore& cs) noexcept
    : m_Db(db)
    , m_CardStore(cs) {
    auto readAll = db.Prepare(SQL::s_ReadAllSQL);
    readAll.Exec();
    while (readAll.Next()) {
        m_DeckRecords.emplace(readAll.Value(0).toULongLong(), dbRead(readAll, false));
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
    if (auto cc = deckStats.CardCount < 0) {
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
