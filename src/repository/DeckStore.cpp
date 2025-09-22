#include "repository/DeckStore.hpp"

#include <expected>

#include "common/Types.hpp"
#include "util/Util.hpp"

[[nodiscard]] Types::IdVector DeckStore::GetDeckIds() const {
    Types::IdVector IdV;
    IdV.reserve(m_DeckCache.size());
    for (auto It = m_DeckCache.begin(); It != m_DeckCache.end(); It++) {
        IdV.push_back(It->second.DeckContext.Id());
    }
    return IdV;
}

void DeckStore::UpsertCache(std::size_t DeckId) {
    m_ReadStmt.Bind(DeckId);
    m_ReadStmt.Exec();
    if (!m_ReadStmt.Next()) {
        Fatal(std::format("Could not read expected deck row id: {}", DeckId));
    }
    DeckContext DeckContext(m_ReadStmt.Value(0).toULongLong());
    if (auto Res = ValidateContextFields(DeckContext); !Res) {
        Fatal(Types::VResultToString(Res));
    }
    DeckStats DeckStats(m_ReadStmt.Value(1).toULongLong());
    if (auto Res = ValidateStatFields(DeckStats); !Res) {
        Fatal(Types::VResultToString(Res));
    }
    DeckInfo DeckInfo(m_ReadStmt.Value(2).toString().toStdString());
    m_ReadStmt.Finish();
    if (auto Res = ValidateInfoFields(DeckInfo); !Res) {
        Fatal(Types::VResultToString(Res));
    }
    Deck       Deck(std::move(DeckInfo), std::move(DeckStats));
    DeckRecord DeckRecord(std::move(DeckContext), std::move(Deck));
    m_DeckCache.insert_or_assign(DeckId, std::move(DeckRecord));
}

DeckStore::DeckStore(const DatabaseQt& Db)
    : m_Db(Db) {
    DatabaseQt::Stmt GetIds = m_Db.Prepare(SQL::s_GetIdsSQL);
    GetIds.Exec();
    while (GetIds.Next()) {
        std::size_t DeckId = GetIds.Value(0).toULongLong();
        UpsertCache(DeckId);
    }
}

[[nodiscard]] DeckStore::ContextVResult
DeckStore::ValidateContextFields(const DeckContext& DeckContext) const {
    Types::ValidationErrors<ContextField> VE;
    if (auto Id = DeckContext.Id() < 0) {
        VE.emplace_back(ContextField::Id, std::format("Invalid deck id: {}", Id));
    }
    if (VE.empty())
        return ContextVResult{};
    return std::unexpected(std::move(VE));
}

[[nodiscard]] DeckStore::StatVResult
DeckStore::ValidateStatFields(const DeckStats& DeckStats) const {
    Types::ValidationErrors<StatField> VE;
    if (auto CC = DeckStats.CardCount() < 0) {
        VE.emplace_back(StatField::CardCount, std::format("Invalid deck card count: {}", CC));
    }
    if (VE.empty())
        return StatVResult{};
    return std::unexpected(std::move(VE));
}

[[nodiscard]] DeckStore::InfoVResult DeckStore::ValidateInfoFields(const DeckInfo& DeckInfo) const {
    Types::ValidationErrors<InfoField> VE;
    if (const auto& S = DeckInfo.Name; S.size() > Limit::s_MAX_NAME_LEN) {
        VE.emplace_back(
            InfoField::Name,
            std::format("Deck name exceeds {} characters: {}", Limit::s_MAX_NAME_LEN, S));
    }
    // TODO: Check duplicate names
    if (VE.empty())
        return InfoVResult{};
    return std::unexpected(std::move(VE));
}

[[nodiscard]] DeckStore::InfoVResult DeckStore::Create(Deck&& Deck) {
    if (auto Res = ValidateInfoFields(Deck.DeckInfo); !Res) {
        return Res;
    }
    m_CreateStmt.Bind(Deck.DeckInfo.Name);
    m_CreateStmt.Exec();
    std::size_t DeckId = m_CreateStmt.LastInsertId();
    m_CreateStmt.Finish();

    UpsertCache(DeckId);
    return InfoVResult{};
}

[[nodiscard]] const Deck* DeckStore::Read(std::size_t DeckId) const noexcept {
    CheckDeckId(DeckId);
    return &m_DeckCache.at(DeckId).Deck;
}

[[nodiscard]] DeckStore::InfoVResult DeckStore::Update(std::size_t DeckId, Deck&& Deck) {
    if (auto Res = ValidateInfoFields(Deck.DeckInfo); !Res) {
        return Res;
    }
    m_UpdateStmt.Bind(Deck.DeckInfo.Name, DeckId);
    m_UpdateStmt.Exec();
    m_UpdateStmt.Finish();

    UpsertCache(DeckId);
    return InfoVResult{};
}

void DeckStore::Delete(std::size_t DeckId) noexcept {
    m_DeleteStmt.Bind(DeckId);
    m_DeleteStmt.Exec();
    m_DeleteStmt.Finish();

    CheckDeckId(DeckId);
    m_DeckCache.erase(DeckId);
}

void DeckStore::CheckDeckId(std::size_t DeckId) const noexcept {
    if (auto It = m_DeckCache.find(DeckId); It == m_DeckCache.end()) {
        Fatal(std::format("Invalid deck id: {}", DeckId));
    }
}
