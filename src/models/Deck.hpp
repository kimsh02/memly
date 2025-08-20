#pragma once

#include <vector>

#include "models/Flashcard.hpp"

class Deck {
public:
    Deck(std::string name)
        : m_Name(std::move(name)) {}

    // Setters
    void SetName(std::string name) { m_Name = std::move(name); }

    // Getters
    const std::string& GetName() const noexcept { return m_Name; }

private:
    std::vector<FlashCard> m_FlashCards;

    std::string m_Name;
};
