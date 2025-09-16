#pragma once

#include <string>

struct DeckInfo {
    std::string Name;

    DeckInfo(const DeckInfo&)                     = delete;
    DeckInfo& operator=(const DeckInfo&) noexcept = delete;

    DeckInfo(DeckInfo&&)                     = default;
    DeckInfo& operator=(DeckInfo&&) noexcept = default;

    explicit DeckInfo(std::string&& name) noexcept
        : Name(std::move(name)) {}
};

struct Deck final {
    DeckInfo DeckInfo;

    Deck(const Deck&)                     = delete;
    Deck& operator=(const Deck&) noexcept = delete;

    Deck(Deck&&)                     = default;
    Deck& operator=(Deck&&) noexcept = default;

    explicit Deck(struct DeckInfo&& deckInfo) noexcept
        : DeckInfo(std::move(deckInfo)) {}
};
