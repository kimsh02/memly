#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

#include "common/Types.hpp"
#include "database_qt/DatabaseQt.hpp"
#include "database_qt/FlashCardRecord.hpp"
#include "models/FlashCard.hpp"
#include "util/Util.hpp"

class DeckStore;

class CardStore final {
public:
    // CardStore(const CardStore&)            = delete;
    // CardStore& operator=(const CardStore&) = delete;

    // CardStore(CardStore&&) noexcept            = delete;
    // CardStore& operator=(CardStore&&) noexcept = delete;

    // explicit CardStore(const DatabaseQt& db, DeckStore& ds) noexcept;

    // enum class ContentField { Front, Back, Audio, Image };

    // using CVResult = Types::VResult<ContentField>;

    // [[nodiscard]] CVResult Create(FlashCard&& card); // Update deck count in memory

    // [[nodiscard]] const FlashCard* Read(std::size_t cardID) noexcept;

    // [[nodiscard]] CVResult Update(std::size_t cardID,
    //                               FlashCard&& card); // Update deck count in memory if applicable

    // void Delete(std::size_t cardID) noexcept; // Update deck count in memory

    // [[nodiscard]] Types::IDVector GetFlashCardIDsByDeckID(std::size_t deckID) const;

private:
    // When cardstore inserts card deck card count needs to updated in memory
    // std::unordered_map<std::size_t, FlashCardRecord> m_CardRecords;

    // std::unordered_map<std::size_t, std::unordered_set<std::size_t>> m_Decks;

    // static bool        validateTimeString(const Types::TimeString& timeString);
    // static bool        validatePathString(const Types::PathString& pathString);
    // static bool        validateCardContent(const CardContent& cardContent);
    // static std::string currentTime(void);

    // bool validateDuplicate(const CardContent& cardContent, std::size_t deckID);

private:
    friend class DeckStore;

    void deleteCardsInDeck(std::size_t deckID) noexcept { Fatal(std::to_string(deckID)); }
};
