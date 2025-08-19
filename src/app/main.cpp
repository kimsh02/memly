#include <print>

#include "flashcard.hpp"

int main() {
    FlashCard card("Front", "Back", "en", "fr", std::nullopt, std::nullopt);
    std::println("Front: {}", card.GetFrontText());
 }
