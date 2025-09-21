#pragma once

#include <string>

struct DeckInfo {
    std::string Name;

    DeckInfo(const DeckInfo&)                     = delete;
    DeckInfo& operator=(const DeckInfo&) noexcept = delete;

    DeckInfo(DeckInfo&&)                     = default;
    DeckInfo& operator=(DeckInfo&&) noexcept = default;

    explicit DeckInfo(std::string&& Name) noexcept
        : Name(std::move(Name)) {}
};

struct Deck final {
    DeckInfo DeckInfo;

    Deck(const Deck&)                     = delete;
    Deck& operator=(const Deck&) noexcept = delete;

    Deck(Deck&&)                     = default;
    Deck& operator=(Deck&&) noexcept = default;

    explicit Deck(struct DeckInfo&& DeckInfo) noexcept
        : DeckInfo(std::move(DeckInfo)) {}
};
