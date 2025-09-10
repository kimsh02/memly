#pragma once

#include <unordered_map>

#include "common/Types.hpp"
#include "database_qt/FlashCardRecord.hpp"
#include "models/FlashCard.hpp"

class CardStore final {
public:
    [[nodiscard]] std::size_t Create(FlashCard&& card);

    // Non-owning pointer invalidated IFF FlashCard is deleted from m_Cards
    [[nodiscard]] const FlashCard* Read(std::size_t cardID) const noexcept;

    [[nodiscard]] bool Update(std::size_t cardID, FlashCard&& card);

    [[nodiscard]] bool Delete(std::size_t cardID) noexcept;

    CardStore(const CardStore&)            = delete;
    CardStore& operator=(const CardStore&) = delete;

    CardStore(CardStore&&) noexcept            = delete;
    CardStore& operator=(CardStore&&) noexcept = delete;

    CardStore();

private:
    std::unordered_map<std::size_t, FlashCardRecord> m_Cards;

    static bool        validateTimeString(const Types::TimeString& timeString);
    static bool        validatePathString(const Types::PathString& pathString);
    static bool        validateCardContent(const CardContent& cardContent);
    static std::string currentTime(void);

    bool validateDuplicate(const CardContent& cardContent, std::size_t deckID);
};
