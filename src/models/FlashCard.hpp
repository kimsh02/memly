#pragma once

#include <cstdint>
#include <string>

#include "common/Types.hpp"

enum class Rating : std::uint8_t { Again, Hard, Good, Easy };

struct ReviewState {
    double Difficulty, Stability;
    int    Reviews, Lapses;

    Types::OptionalTimeString Due;

    ReviewState(const ReviewState&)            = delete;
    ReviewState& operator=(const ReviewState&) = delete;

    ReviewState(ReviewState&&) noexcept            = default;
    ReviewState& operator=(ReviewState&&) noexcept = delete;

    explicit ReviewState(double                      difficulty = 5.0,
                         double                      stability  = 1.0,
                         int                         reviews    = 0,
                         int                         lapses     = 0,
                         Types::OptionalTimeString&& due        = std::nullopt) noexcept
        : Difficulty(difficulty)
        , Stability(stability)
        , Reviews(reviews)
        , Lapses(lapses)
        , Due(std::move(due)) {}
};

struct AudioResource {
    Types::PathString NormalSpeed, SlowSpeed;

    AudioResource(const AudioResource&)            = delete;
    AudioResource& operator=(const AudioResource&) = delete;

    AudioResource(AudioResource&&) noexcept            = default;
    AudioResource& operator=(AudioResource&&) noexcept = delete;

    AudioResource(Types::PathString&& normalSpeed, Types::PathString&& slowSpeed) noexcept
        : NormalSpeed(std::move(normalSpeed))
        , SlowSpeed(std::move(slowSpeed)) {}
};

struct CardContent {
    using OptionalAudioResource = std::optional<AudioResource>;

    std::string FrontText, BackText;

    OptionalAudioResource     Audio;
    Types::OptionalPathString Image;

    CardContent(const CardContent&)            = delete;
    CardContent& operator=(const CardContent&) = delete;

    CardContent(CardContent&&) noexcept            = default;
    CardContent& operator=(CardContent&&) noexcept = delete;

    explicit CardContent(std::string&&               frontText,
                         std::string&&               backText,
                         OptionalAudioResource&&     audio,
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

    explicit FlashCard(CardContent&& content, ReviewState&& state = ReviewState()) noexcept
        : Content(std::move(content))
        , State(std::move(state)) {}
};
