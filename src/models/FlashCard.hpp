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

    explicit ReviewState(double                      Difficulty,
                         double                      Stability,
                         int                         Reviews,
                         int                         Lapses,
                         Types::OptionalTimeString&& Due = std::nullopt) noexcept
        : Difficulty(Difficulty)
        , Stability(Stability)
        , Reviews(Reviews)
        , Lapses(Lapses)
        , Due(std::move(Due)) {}
};

struct CardContent {
    std::string FrontText, BackText;

    Types::PathString         Audio;
    Types::OptionalPathString Image;

    CardContent(const CardContent&)            = delete;
    CardContent& operator=(const CardContent&) = delete;

    CardContent(CardContent&&) noexcept            = default;
    CardContent& operator=(CardContent&&) noexcept = delete;

    explicit CardContent(std::string&&               FrontText,
                         std::string&&               BackText,
                         Types::PathString&&         Audio,
                         Types::OptionalPathString&& Image = std::nullopt) noexcept
        : FrontText(std::move(FrontText))
        , BackText(std::move(BackText))
        , Audio(std::move(Audio))
        , Image(std::move(Image)) {}
};

struct FlashCard final {
    CardContent Content;
    ReviewState State;

    FlashCard(const FlashCard&)            = delete;
    FlashCard& operator=(const FlashCard&) = delete;

    FlashCard(FlashCard&&) noexcept            = default;
    FlashCard& operator=(FlashCard&&) noexcept = delete;

    explicit FlashCard(CardContent&& Content, ReviewState&& State) noexcept
        : Content(std::move(Content))
        , State(std::move(State)) {}
};
