#pragma once

#include <string>

struct DeckInfo {
    std::string       Name;
    const std::size_t CardCount;
};

struct Deck {
    const std::size_t Id;

    DeckInfo DeckInfo;
};
