#pragma once

#include <vector>

#include "repetition_ai/flashcard.hpp"

class Deck {
public:
    Deck() = default;

private:
    std::vector<FlashCard> m_FlashCards;

    std::string m_Name;
};
