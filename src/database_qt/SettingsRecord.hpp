#pragma once

#include "models/Settings.hpp"

struct SettingsContext {
    const std::size_t ID;

    SettingsContext(const SettingsContext&)            = delete;
    SettingsContext& operator=(const SettingsContext&) = delete;

    SettingsContext(SettingsContext&&) noexcept            = default;
    SettingsContext& operator=(SettingsContext&&) noexcept = delete;

    explicit SettingsContext(std::size_t id) noexcept
        : ID(id) {}
};

struct SettingsRecord final {
    SettingsContext SettingsContext;
    Settings        Settings;

    SettingsRecord(const SettingsRecord&)            = delete;
    SettingsRecord& operator=(const SettingsRecord&) = delete;

    SettingsRecord(SettingsRecord&&) noexcept            = delete;
    SettingsRecord& operator=(SettingsRecord&&) noexcept = delete;

    explicit SettingsRecord(struct SettingsContext&& settingsContext,
                            struct Settings&&        settings) noexcept
        : SettingsContext(std::move(settingsContext))
        , Settings(std::move(settings)) {}
};
