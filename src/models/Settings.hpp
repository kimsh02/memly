#pragma once

#include <string>

struct AppSettings {
    std::size_t TargetLangIdx;
};

struct UserSettings {
    std::string Name;
};

struct Settings {
    const std::size_t Id;

    AppSettings  AppSettings;
    UserSettings UserSettings;
};
