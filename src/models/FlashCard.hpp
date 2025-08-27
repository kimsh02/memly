#pragma once

#include <cstdint>
#include <string>

#include "common/Types.hpp"

enum class Rating : std::uint8_t { Again, Hard, Good, Easy };

struct ReviewState {
    double Difficulty = 5.0;
    double Stability  = 1.0;
    int    Reviews    = 0;
    int    Lapses     = 0;

    Types::OptionalTimeString Due = std::nullopt;

    ReviewState() = default;

    ReviewState(const ReviewState&)            = delete;
    ReviewState& operator=(const ReviewState&) = delete;

    ReviewState(ReviewState&&) noexcept            = default;
    ReviewState& operator=(ReviewState&&) noexcept = delete;
};

struct CardContent {
    std::string FrontText, BackText;

    Types::OptionalPathString Audio, Image;

    CardContent(const CardContent&)            = delete;
    CardContent& operator=(const CardContent&) = delete;

    CardContent(CardContent&&) noexcept            = default;
    CardContent& operator=(CardContent&&) noexcept = delete;

    explicit CardContent(std::string&&               frontText,
                         std::string&&               backText,
                         Types::OptionalPathString&& audio,
                         Types::OptionalPathString&& image) noexcept
        : FrontText(std::move(frontText))
        , BackText(std::move(backText))
        , Audio(std::move(audio))
        , Image(std::move(image)) {}
};

struct FlashCard final {
    CardContent Content;
    ReviewState State;

    FlashCard(const FlashCard&)            = delete;
    FlashCard& operator=(const FlashCard&) = delete;

    FlashCard(FlashCard&&) noexcept            = default;
    FlashCard& operator=(FlashCard&&) noexcept = delete;

    explicit FlashCard(CardContent&& content) noexcept
        : Content(std::move(content)) {}
};
