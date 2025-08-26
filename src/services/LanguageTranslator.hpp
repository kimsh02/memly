#pragma once

#include <array>
#include <cstddef>
#include <span>
#include <string_view>

class LanguageTranslator final {
public:
    LanguageTranslator(const LanguageTranslator&)            = delete;
    LanguageTranslator& operator=(const LanguageTranslator&) = delete;

    LanguageTranslator(LanguageTranslator&&) noexcept            = delete;
    LanguageTranslator& operator=(LanguageTranslator&&) noexcept = delete;

    LanguageTranslator() noexcept {
        loadLastLanguageSrcUsedIndex();
        loadLastLanguageDstUsedIndex();
    }

    [[nodiscard]] std::string Translate(std::string_view text) const;

    [[nodiscard]] std::span<const std::string_view> GetLatinLangNames() const noexcept {
        return s_LatinLangNames;
    }

    [[nodiscard]] std::string_view GetLangSrcName() const noexcept {
        return s_LatinLangNames[m_LanguageSrcIndex];
    }

    [[nodiscard]] std::string_view GetLangDstName() const noexcept {
        return s_LatinLangNames[m_LanguageDstIndex];
    }

    [[nodiscard]] std::size_t GetLangSrcIndex() const noexcept { return m_LanguageSrcIndex; }

    [[nodiscard]] std::size_t GetLangDstIndex() const noexcept { return m_LanguageDstIndex; }

private:
    inline static constexpr std::array<std::string_view, 60> s_LatinLangCodes = {
        "af",  "sq", "eu", "bs", "ca",  "hr", "cs", "da", "nl", "en", "et",  "fil",
        "fi",  "fr", "fy", "gl", "de",  "ht", "hu", "is", "id", "ga", "it",  "la",
        "lv",  "lt", "lb", "ms", "mt",  "mi", "no", "oc", "pl", "pt", "qu",  "ro",
        "rom", "sm", "gd", "st", "scn", "sk", "sl", "so", "es", "su", "sw",  "sv",
        "tet", "ts", "tn", "tr", "tk",  "ak", "vi", "cy", "xh", "yo", "yua", "zu"
    };

    inline static constexpr std::array<std::string_view, s_LatinLangCodes.size()> s_LatinLangNames = {
        "Afrikaans",  "Albanian",   "Basque",        "Bosnian",    "Catalan",      "Croatian",
        "Czech",      "Danish",     "Dutch",         "English",    "Estonian",     "Filipino",
        "Finnish",    "French",     "Frisian",       "Galician",   "German",       "Haitian Creole",
        "Hungarian",  "Icelandic",  "Indonesian",    "Irish",      "Italian",      "Latin",
        "Latvian",    "Lithuanian", "Luxembourgish", "Malay",      "Maltese",      "Maori",
        "Norwegian",  "Occitan",    "Polish",        "Portuguese", "Quechua",      "Romanian",
        "Romani",     "Samoan",     "Scots Gaelic",  "Sesotho",    "Sicilian",     "Slovak",
        "Slovenian",  "Somali",     "Spanish",       "Sundanese",  "Swahili",      "Swedish",
        "Tetum",      "Tsonga",     "Tswana",        "Turkish",    "Turkmen",      "Twi",
        "Vietnamese", "Welsh",      "Xhosa",         "Yoruba",     "Yucatec Maya", "Zulu"
    };
    static_assert(s_LatinLangCodes.size() == s_LatinLangNames.size());

    std::size_t m_LanguageSrcIndex;
    std::size_t m_LanguageDstIndex;

    void loadLastLanguageSrcUsedIndex() noexcept;
    void loadLastLanguageDstUsedIndex() noexcept;

    void setLanguageSrcIndex(std::size_t index) noexcept;
    void setLanguageDstIndex(std::size_t index) noexcept;
};
