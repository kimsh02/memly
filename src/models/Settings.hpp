#pragma once

#include <string>

struct AppSettings {
    std::size_t TargetLangIdx;
};

struct UserSettings {
    std::string Name;
};

enum class UserField { Name };
enum class AppField { TargetLangIdx };

struct Settings {
    const std::size_t Id;

    AppSettings  AppSettings;
    UserSettings UserSettings;
};
