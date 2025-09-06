#pragma once

#include <cstdint>
#include <optional>
#include <string>

#include "common/Types.hpp"

enum class Rating : std::uint8_t { Again, Hard, Good, Easy };

struct ReviewState {
    double Difficulty;
    double Stability;
    int    Reviews;
    int    Lapses;

    Types::OptionalTimeString Due;

    ReviewState(const ReviewState&)            = delete;
    ReviewState& operator=(const ReviewState&) = delete;

    ReviewState(ReviewState&&) noexcept            = default;
    ReviewState& operator=(ReviewState&&) noexcept = delete;

    explicit ReviewState(double                      difficulty,
                         double                      stability,
                         int                         reviews,
                         int                         lapses,
                         Types::OptionalTimeString&& due = std::nullopt) noexcept
        : Difficulty(difficulty)
        , Stability(stability)
        , Reviews(reviews)
        , Lapses(lapses)
        , Due(std::move(due)) {}
};

struct CardContent {
    std::string FrontText, BackText;

    Types::PathString         Audio;
    Types::OptionalPathString Image;

    CardContent(const CardContent&)            = delete;
    CardContent& operator=(const CardContent&) = delete;

    CardContent(CardContent&&) noexcept            = default;
    CardContent& operator=(CardContent&&) noexcept = delete;

    explicit CardContent(std::string&&               frontText,
                         std::string&&               backText,
                         Types::PathString&&         audio,
                         Types::OptionalPathString&& image = std::nullopt) noexcept
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

    explicit FlashCard(CardContent&& content, ReviewState&& state) noexcept
        : Content(std::move(content))
        , State(std::move(state)) {}
};
