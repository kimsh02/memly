// #pragma once

// #include <unordered_map>

// #include "common/Types.hpp"
// #include "database_qt/DatabaseQt.hpp"
// #include "database_qt/DeckRecord.hpp"
// #include "models/Deck.hpp"

// class DeckStore final {
// public:
//     DeckStore(const DeckStore&)            = delete;
//     DeckStore& operator=(const DeckStore&) = delete;

//     DeckStore(DeckStore&&) noexcept            = delete;
//     DeckStore& operator=(DeckStore&&) noexcept = delete;

//     explicit DeckStore(const DatabaseQt& Db);

//     enum class InfoField { Name };

//     using InfoVResult = Types::VResult<InfoField>;

//     [[nodiscard]] InfoVResult Create(Deck&& Deck);

//     [[nodiscard]] const Deck* Read(std::size_t DeckId) const noexcept;

//     [[nodiscard]] InfoVResult Update(std::size_t DeckId, Deck&& Deck);

//     void Delete(std::size_t DeckId) noexcept;

//     [[nodiscard]] Types::IdVector GetDeckIds() const;

// private:
//     struct SQL {
//         inline static constexpr auto s_CreateSQL = R"(
//             INSERT INTO decks(name)
//             VALUES(?);)";
//         inline static constexpr auto s_GetIdsSQL = R"(
//             SELECT id FROM decks;)";
//         inline static constexpr auto s_ReadSQL   = R"(
//             SELECT * FROM decks
//             WHERE id = ?;)";
//         inline static constexpr auto s_UpdateSQL = R"(
//             UPDATE decks
//             SET name = ?
//             WHERE id = ?;)";
//         inline static constexpr auto s_DeleteSQL = R"(
//             DELETE FROM decks
//             WHERE id = ?;)";
//     };

//     struct Limit {
//         inline static constexpr std::size_t s_MAX_NAME_LEN = 80;
//     };

//     const DatabaseQt& m_Db;

//     DatabaseQt::Stmt m_CreateStmt = m_Db.Prepare(SQL::s_CreateSQL);
//     DatabaseQt::Stmt m_ReadStmt   = m_Db.Prepare(SQL::s_ReadSQL);
//     DatabaseQt::Stmt m_UpdateStmt = m_Db.Prepare(SQL::s_UpdateSQL);
//     DatabaseQt::Stmt m_DeleteStmt = m_Db.Prepare(SQL::s_DeleteSQL);

//     std::unordered_map<std::size_t, DeckRecord> m_DeckCache;

//     void CheckDeckId(std::size_t DeckId) const noexcept;

//     void UpsertCache(std::size_t DeckId);

//     enum class StatField { CardCount };
//     enum class ContextField { Id };

//     using StatVResult    = Types::VResult<StatField>;
//     using ContextVResult = Types::VResult<ContextField>;

//     [[nodiscard]] ContextVResult ValidateContextFields(const DeckContext& DeckContext) const;
//     [[nodiscard]] StatVResult    ValidateStatFields(const DeckStats& DeckStats) const;
//     [[nodiscard]] InfoVResult    ValidateInfoFields(const DeckInfo& DeckInfo) const;
// };
