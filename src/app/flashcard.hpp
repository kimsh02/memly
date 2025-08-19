#pragma once

#include <optional>
#include <string>

using PathString = std::string;

class FlashCard {
public:
    FlashCard() = default;

    explicit FlashCard(std::string               frontText,
                       std::string               backText,
                       std::string               langSrc,
                       std::string               langDst,
                       std::optional<PathString> audioPath,
                       std::optional<PathString> imagePath)
        : m_FrontText(std::move(frontText))
        , m_BackText(std::move(backText))
        , m_LangSrc(std::move(langSrc))
        , m_LangDst(std::move(langDst))
        , m_AudioPath(std::move(audioPath))
        , m_ImagePath(std::move(imagePath)) {}

    // Setters
    void SetFrontText(std::string text) { m_FrontText = std::move(text); }

    void SetBackText(std::string text) { m_BackText = std::move(text); }

    void SetLangSrc(std::string lang) { m_LangSrc = std::move(lang); }

    void SetLangDst(std::string lang) { m_LangDst = std::move(lang); }

    void SetAudioPath(std::optional<PathString> path) { m_AudioPath = std::move(path); }

    void SetImagePath(std::optional<PathString> path) { m_ImagePath = std::move(path); }

    // Getters
    const std::string& GetFrontText() const noexcept { return m_FrontText; }

    const std::string& GetBackText() const noexcept { return m_BackText; }

    const std::string& GetLangSrc() const noexcept { return m_LangSrc; }

    const std::string& GetLangDst() const noexcept { return m_LangDst; }

    const std::optional<PathString>& GetAudioPath() const noexcept { return m_AudioPath; }

    const std::optional<PathString>& GetImagePath() const noexcept { return m_ImagePath; }

    // Operators
    auto operator<=>(const FlashCard& other) const noexcept {
        return m_BackText <=> other.m_BackText;
    }

    bool operator==(const FlashCard& other) const noexcept {
        return m_BackText == other.m_BackText;
    }

private:
    std::string m_FrontText;
    std::string m_BackText;
    std::string m_LangSrc;
    std::string m_LangDst;

    std::optional<PathString> m_AudioPath;
    std::optional<PathString> m_ImagePath;
};
