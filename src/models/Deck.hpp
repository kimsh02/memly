#pragma once

#include <string>

struct DeckStats {
    std::size_t CardCount;

    DeckStats(const DeckStats&)                     = delete;
    DeckStats& operator=(const DeckStats&) noexcept = delete;

    DeckStats(DeckStats&&)                     = default;
    DeckStats& operator=(DeckStats&&) noexcept = default;

    explicit DeckStats(std::size_t cardCount) noexcept
        : CardCount(cardCount) {}
};

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
    DeckStats DeckStats;
    DeckInfo  DeckInfo;

    Deck(const Deck&)                     = delete;
    Deck& operator=(const Deck&) noexcept = delete;

    Deck(Deck&&)                     = default;
    Deck& operator=(Deck&&) noexcept = default;

    explicit Deck(struct DeckStats&& deckStats, struct DeckInfo&& deckInfo) noexcept
        : DeckStats(std::move(deckStats))
        , DeckInfo(std::move(deckInfo)) {}
};
