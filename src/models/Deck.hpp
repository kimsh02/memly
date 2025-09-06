#pragma once

#include <string>

struct Deck final {
    std::string Name;
    std::size_t CardCount;

    Deck(const Deck&)                     = delete;
    Deck& operator=(const Deck&) noexcept = delete;

    Deck(Deck&&)                     = default;
    Deck& operator=(Deck&&) noexcept = delete;

    explicit Deck(std::string&& name, std::size_t cardCount) noexcept
        : Name(std::move(name))
        , CardCount(cardCount) {}
};
