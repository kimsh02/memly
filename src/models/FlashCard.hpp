#pragma once

#include <cstdint>
#include <optional>
#include <string>

enum class Rating : std::uint8_t { Again, Hard, Good, Easy };

struct ReviewState {
    using DayString = std::string; // std::chrono::sys_days
    double Difficulty, Stability;
    int    Reviews, Lapses;

    std::optional<DayString> Due;

    ReviewState(const ReviewState&)            = delete;
    ReviewState& operator=(const ReviewState&) = delete;

    ReviewState(ReviewState&&) noexcept            = default;
    ReviewState& operator=(ReviewState&&) noexcept = default;

    explicit ReviewState(double                     difficulty = 5.0,
                         double                     stability  = 1.0,
                         int                        reviews    = 0,
                         int                        lapses     = 0,
                         std::optional<DayString>&& due        = std::nullopt) noexcept
        : Difficulty(difficulty)
        , Stability(stability)
        , Reviews(reviews)
        , Lapses(lapses)
        , Due(std::move(due)) {}
};

struct CardContent {
    using PathString = std::string; // std::filesystem::path
    std::string FrontText, BackText, LangSrc, LangDst;

    std::optional<PathString> AudioPath, ImagePath;

    CardContent(const CardContent&)            = delete;
    CardContent& operator=(const CardContent&) = delete;

    CardContent(CardContent&&) noexcept            = default;
    CardContent& operator=(CardContent&&) noexcept = default;

    explicit CardContent(std::string&&               frontText,
                         std::string&&               backText,
                         std::string&&               langSrc,
                         std::string&&               langDst,
                         std::optional<PathString>&& audioPath,
                         std::optional<PathString>&& imagePath) noexcept
        : FrontText(std::move(frontText))
        , BackText(std::move(backText))
        , LangSrc(std::move(langSrc))
        , LangDst(std::move(langDst))
        , AudioPath(std::move(audioPath))
        , ImagePath(std::move(imagePath)) {}
};

struct FlashCard final {
    CardContent Content;
    ReviewState State;

    FlashCard(const FlashCard&)            = delete;
    FlashCard& operator=(const FlashCard&) = delete;

    FlashCard(FlashCard&&) noexcept            = default;
    FlashCard& operator=(FlashCard&&) noexcept = default;

    explicit FlashCard(CardContent&& content, ReviewState&& state = ReviewState()) noexcept
        : Content(std::move(content))
        , State(std::move(state)) {}
};
