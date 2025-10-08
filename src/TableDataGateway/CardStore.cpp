// #include "repository/CardStore.hpp"

// #include <chrono>
// #include <filesystem>

// #include "common/Types.hpp"
// #include "models/FlashCard.hpp"

// #include "repository/DeckStore.hpp"

// [[nodiscard]] Types::ID CardStore::Create(const Types::ID& deckID, CardContent&& cardContent) {}

// [[nodiscard]] const FlashCard* CardStore::Read(const Types::ID& cardID) const noexcept {
//     if (auto it = m_Cards.find(cardID); it != m_Cards.end()) { return &it->second.Card; }
//     return nullptr;
// }

// template <typename Func>
//     requires std::invocable<Func, FlashCard&>
// [[nodiscard]] bool CardStore::Update(const Types::ID& cardID, Func&& updateFunc) {
//     if (auto it = m_Cards.find(cardID); it != m_Cards.end()) {
//         updateFunc(it->second.FlashCard);
//         return true;
//     }
//     return false;
// }

// [[nodiscard]] bool CardStore::Delete(const Types::ID& cardID) noexcept { return true; }

// bool CardStore::validateTimeString(const Types::TimeString& timeString) {}

// bool CardStore::validatePathString(const Types::PathString& pathString) {}

// bool CardStore::validateCardContent(const CardContent& cardContent) {
//     if (cardContent.FrontText.size() > 80) {
//         return false;
//     } else if (cardContent.BackText.size() > 80) {
//         return false;
//     }
// }

// bool CardStore::validateDuplicate(const CardContent& cardContent, const Types::ID& deckID) {
//     for (auto& [_, flashCardRecord] : m_Cards) {
//         if (flashCardRecord.FlashCardContext.DeckID == deckID) {
//             const CardContent& other = flashCardRecord.FlashCard.Content;
//             if (cardContent.BackText == other.BackText) { return false; }
//         }
//     }
//     return true;
// }

// void CardStore::loadAllCards(void) {}
