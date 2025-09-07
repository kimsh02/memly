#pragma once

#include "models/Settings.hpp"

struct SettingsContext {
public:
    SettingsContext(const SettingsContext&)            = delete;
    SettingsContext& operator=(const SettingsContext&) = delete;

    SettingsContext(SettingsContext&&) noexcept            = default;
    SettingsContext& operator=(SettingsContext&&) noexcept = default;

    explicit SettingsContext(std::size_t id) noexcept
        : m_ConstID(id) {}

    std::size_t ID() const noexcept { return m_ConstID; }

private:
    std::size_t m_ConstID;
};

struct SettingsRecord final {
    SettingsContext SettingsContext;
    Settings        Settings;

    SettingsRecord(const SettingsRecord&)            = delete;
    SettingsRecord& operator=(const SettingsRecord&) = delete;

    SettingsRecord(SettingsRecord&&) noexcept            = default;
    SettingsRecord& operator=(SettingsRecord&&) noexcept = default;

    explicit SettingsRecord(struct SettingsContext&& settingsContext,
                            struct Settings&&        settings) noexcept
        : SettingsContext(std::move(settingsContext))
        , Settings(std::move(settings)) {}
};
