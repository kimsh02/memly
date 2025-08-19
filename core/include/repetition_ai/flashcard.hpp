#pragma once
#include <filesystem>
#include <optional>
#include <string>

class FlashCard {
public:
    using path = std::filesystem::path;

    FlashCard() = default;

    // Pass-by-value setters
    void SetFrontText(std::string text) noexcept { m_FrontText = std::move(text); }

    void SetBackText(std::string text) noexcept { m_BackText = std::move(text); }

    void SetLangSrc(std::string lang) noexcept { m_LangSrc = std::move(lang); }

    void SetLangDest(std::string lang) noexcept { m_LangDest = std::move(lang); }

    void SetAudioReference(path p) noexcept { m_AudioPath = std::move(p); }

    void SetImageReference(path p) noexcept { m_ImagePath = std::move(p); }

    // Clear references
    void ClearAudioReference() noexcept { m_AudioPath.reset(); }

    void ClearImageReference() noexcept { m_ImagePath.reset(); }

    // Getters
    const std::string& GetFrontText() const noexcept { return m_FrontText; }

    const std::string& GetBackText() const noexcept { return m_BackText; }

    const std::string& GetLangSrc() const noexcept { return m_LangSrc; }

    const std::string& GetLangDest() const noexcept { return m_LangDest; }

    const std::optional<path>& GetAudioPath() const noexcept { return m_AudioPath; }

    const std::optional<path>& GetImagePath() const noexcept { return m_ImagePath; }

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
    std::string m_LangDest;

    std::optional<path> m_AudioPath;
    std::optional<path> m_ImagePath;
};
