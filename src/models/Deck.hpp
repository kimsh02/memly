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

struct DeckStats {
    DeckStats(const DeckStats&)                     = delete;
    DeckStats& operator=(const DeckStats&) noexcept = delete;

    DeckStats(DeckStats&&)                     = default;
    DeckStats& operator=(DeckStats&&) noexcept = default;

    explicit DeckStats(std::size_t CardCount) noexcept
        : m_ConstCardCount(CardCount) {}

    [[nodiscard]] std::size_t CardCount() const noexcept { return m_ConstCardCount; }

private:
    std::size_t m_ConstCardCount;
};

struct Deck final {
    DeckInfo  DeckInfo;
    DeckStats DeckStats;

    Deck(const Deck&)                     = delete;
    Deck& operator=(const Deck&) noexcept = delete;

    Deck(Deck&&)                     = default;
    Deck& operator=(Deck&&) noexcept = default;

    explicit Deck(struct DeckInfo&& DeckInfo, struct DeckStats&& DeckStats) noexcept
        : DeckInfo(std::move(DeckInfo))
        , DeckStats(std::move(DeckStats)) {}
};
