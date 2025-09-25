#pragma once

#include <string>

#include "common/Types.hpp"

enum class Rating : std::uint8_t { Again, Hard, Good, Easy };

struct ReviewState {
    float Difficulty;
    float Stability;
};

struct CardContent {
    std::string               FrontText;
    std::string               BackText;
    Types::PathString         Audio;
    Types::OptionalPathString Image;
};

struct Flashcard {
    const std::size_t Id;
    const std::size_t Deck_Id;

    CardContent Content;
    ReviewState State;
};
