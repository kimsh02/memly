#pragma once

#include <unordered_map>

#include "common/Types.hpp"
#include "database_qt/DatabaseQt.hpp"
#include "database_qt/FlashCardRecord.hpp"
#include "models/FlashCard.hpp"

class CardStore final {
public:
    CardStore(const CardStore&)            = delete;
    CardStore& operator=(const CardStore&) = delete;

    CardStore(CardStore&&) noexcept            = delete;
    CardStore& operator=(CardStore&&) noexcept = delete;

    explicit CardStore(const DatabaseQt& Db) noexcept;

    enum class ContentField { Front, Back, Audio, Image };

    using ContentVResult = Types::VResult<ContentField>;

    [[nodiscard]] ContentVResult Create(FlashCard&& Card); // TODO: Update deck count in memory

    [[nodiscard]] const FlashCard* Read(std::size_t CardId) noexcept;

    [[nodiscard]] ContentVResult
    Update(std::size_t CardId,
           FlashCard&& Card); // TODO: Update deck count in memory if applicable

    void Delete(std::size_t CardId) noexcept; // TODO: Update deck count in memory

    [[nodiscard]] Types::IdVector GetFlashCardIdsByDeckId(std::size_t DeckId) const;

private:
    struct Default {}; // TODO: Needs SRS

    struct SQL { // TODO: Change
        inline static constexpr auto s_CreateSQL = R"(
            INSERT INTO decks(name)
            VALUES(?);)";
        inline static constexpr auto s_GetIdsSQL = R"(
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
        inline static constexpr std::size_t s_MAX_TEXT_LEN = 80;
    };

    const DatabaseQt& m_Db;

    DatabaseQt::Stmt m_CreateStmt = m_Db.Prepare(SQL::s_CreateSQL);
    DatabaseQt::Stmt m_ReadStmt   = m_Db.Prepare(SQL::s_ReadSQL);
    DatabaseQt::Stmt m_UpdateStmt = m_Db.Prepare(SQL::s_UpdateSQL);
    DatabaseQt::Stmt m_DeleteStmt = m_Db.Prepare(SQL::s_DeleteSQL);

    // When cardstore inserts card deck card count needs to updated in memory
    std::unordered_map<std::size_t, std::unordered_map<std::size_t, FlashCardRecord>> m_CardCache;

    void CheckCardId(std::size_t CardId) const noexcept;

    void UpsertCache(std::size_t CardId);

    enum class ReviewField {};
    enum class ContextField {};

    using ReviewVResult  = Types::VResult<ReviewField>;
    using ContextVResult = Types::VResult<ContextField>;

    [[nodiscard]] ContextVResult
    ValidateContextFields(const FlashCardContext& FlashCardContext) const;
    [[nodiscard]] ReviewVResult  ValidateReviewFields(const ReviewState& ReviewState) const;
    [[nodiscard]] ContentVResult ValidateContentFields(const CardContent& CardContent) const;

    // TODO:
    // static bool        validateTimeString(const Types::TimeString& timeString);
    // static bool        validatePathString(const Types::PathString& pathString);
    // static std::string currentTime(void);
    // bool CheckDuplicate(const CardContent& cardContent, std::size_t deckID);
};
