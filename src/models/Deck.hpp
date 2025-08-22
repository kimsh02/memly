#pragma once

#include <string>

struct Deck final {
    std::string Name;

    Deck(const Deck&)                     = delete;
    Deck& operator=(const Deck&) noexcept = delete;

    Deck(Deck&&)                     = default;
    Deck& operator=(Deck&&) noexcept = delete;

    explicit Deck(std::string&& name) noexcept
        : Name(std::move(name)) {}
};
