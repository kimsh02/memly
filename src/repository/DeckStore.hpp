#pragma once

#include <unordered_map>

#include "common/Types.hpp"
#include "database_qt/DatabaseQt.hpp"
#include "database_qt/DeckRecord.hpp"
#include "models/Deck.hpp"

class DeckStore final {
public:
    DeckStore(const DeckStore&)            = delete;
    DeckStore& operator=(const DeckStore&) = delete;

    DeckStore(DeckStore&&) noexcept            = delete;
    DeckStore& operator=(DeckStore&&) noexcept = delete;

    explicit DeckStore(const DatabaseQt& db) noexcept;

    enum class InfoField { Name };

    using IVResult = Types::VResult<InfoField>;

    [[nodiscard]] IVResult Create(Deck&& deck);

    [[nodiscard]] const Deck* Read(std::size_t deckID) noexcept;

    [[nodiscard]] IVResult Update(std::size_t deckID, Deck&& deck);

    void Delete(std::size_t deckID) noexcept;

    [[nodiscard]] Types::IDVector GetDeckIDs() const;

private:
    struct SQL {
        inline static constexpr auto s_CreateSQL = R"(
            INSERT INTO decks(name)
            VALUES(?);)";
        inline static constexpr auto s_GetIDsSQL = R"(
            SELECT id FROM decks;)";
        inline static constexpr auto s_ReadSQL   = R"(
            SELECT * FROM decks
            WHERE id = ?;)";
        inline static constexpr auto s_UpdateSQL = R"(
            UPDATE decks
            SET name = ?
            WHERE id = ?;)";
        inline static constexpr auto s_DeleteSQL = R"(
            DELETE FROM decks
            WHERE id = ?;)";
    };

    struct Limit {
        inline static constexpr std::size_t s_MAX_NAME_LEN = 80;
    };

    const DatabaseQt& m_Db;

    DatabaseQt::Stmt m_CreateStmt = m_Db.Prepare(SQL::s_CreateSQL);
    DatabaseQt::Stmt m_ReadStmt   = m_Db.Prepare(SQL::s_ReadSQL);
    DatabaseQt::Stmt m_UpdateStmt = m_Db.Prepare(SQL::s_UpdateSQL);
    DatabaseQt::Stmt m_DeleteStmt = m_Db.Prepare(SQL::s_DeleteSQL);

    using DeckRecordMap = std::unordered_map<std::size_t, DeckRecord>;

    DeckRecordMap m_DeckRecords;

    [[nodiscard]] DeckRecordMap::iterator getDeckRecordMapIter(std::size_t deckID) noexcept;

    DeckRecord dbRead(std::size_t deckID);

    enum class StatField { CardCount };
    enum class ContextField { ID };

    using SVResult = Types::VResult<StatField>;
    using CVResult = Types::VResult<ContextField>;

    [[nodiscard]] CVResult validateContextFields(const DeckContext& deckContext) const;
    [[nodiscard]] SVResult validateStatFields(const DeckStats& deckStats) const;
    [[nodiscard]] IVResult validateInfoFields(const DeckInfo& deckInfo) const;
};
