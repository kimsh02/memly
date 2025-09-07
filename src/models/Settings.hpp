#pragma once

#include <string>

struct AppSettings {
    std::size_t LastTargetLanguageIndex;

    AppSettings(const AppSettings&)            = delete;
    AppSettings& operator=(const AppSettings&) = delete;

    AppSettings(AppSettings&&) noexcept            = default;
    AppSettings& operator=(AppSettings&&) noexcept = default;

    explicit AppSettings(std::size_t lastTargetLanguageIndex) noexcept
        : LastTargetLanguageIndex(lastTargetLanguageIndex) {}
};

struct UserSettings {
    std::string Name;

    UserSettings(const UserSettings&)            = delete;
    UserSettings& operator=(const UserSettings&) = delete;

    UserSettings(UserSettings&&) noexcept            = default;
    UserSettings& operator=(UserSettings&&) noexcept = default;

    explicit UserSettings(std::string&& name) noexcept
        : Name(std::move(name)) {}
};

struct Settings final {
    AppSettings  AppSettings;
    UserSettings UserSettings;

    Settings(const Settings&)            = delete;
    Settings& operator=(const Settings&) = delete;

    Settings(Settings&&) noexcept            = default;
    Settings& operator=(Settings&&) noexcept = default;

    explicit Settings(struct AppSettings&& appSettings, struct UserSettings&& userSettings) noexcept
        : AppSettings(std::move(appSettings))
        , UserSettings(std::move(userSettings)) {}
};
