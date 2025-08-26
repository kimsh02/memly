#pragma once

#include <unordered_map>

#include "common/Types.hpp"
#include "database/FlashCardRecord.hpp"
#include "models/FlashCard.hpp"

class CardStore final {
public:
    [[nodiscard]] Types::ID Create(const Types::ID& deckID, CardContent&& cardContent);

    // Non-owning pointer invalidated IFF FlashCard is deleted from m_Cards
    [[nodiscard]] const FlashCard* Read(const Types::ID& cardID) const noexcept;

    template <typename Func>
        requires std::invocable<Func, FlashCard&>
    [[nodiscard]] bool Update(const Types::ID& cardID, Func&& updateFunc);

    [[nodiscard]] bool Delete(const Types::ID& cardID) noexcept;

    CardStore(const CardStore&)            = delete;
    CardStore& operator=(const CardStore&) = delete;

    CardStore(CardStore&&) noexcept            = delete;
    CardStore& operator=(CardStore&&) noexcept = delete;

    explicit CardStore() { loadAllCards(); }

private:
    std::unordered_map<Types::ID, FlashCardRecord> m_Cards;

    static bool        validateTimeString(const Types::TimeString& timeString);
    static bool        validatePathString(const Types::PathString& pathString);
    static bool        validateCardContent(const CardContent& cardContent);
    static std::string currentTime(void);

    bool validateDuplicate(const CardContent& cardContent, const Types::ID& deckID);

    void loadAllCards();
};
