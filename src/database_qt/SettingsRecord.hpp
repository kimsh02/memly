#pragma once

#include "models/Settings.hpp"

struct SettingsContext {
public:
    SettingsContext(const SettingsContext&)            = delete;
    SettingsContext& operator=(const SettingsContext&) = delete;

    SettingsContext(SettingsContext&&) noexcept            = default;
    SettingsContext& operator=(SettingsContext&&) noexcept = default;

    explicit SettingsContext(std::size_t Id) noexcept
        : m_ConstId(Id) {}

    [[nodiscard]] std::size_t Id() const noexcept { return m_ConstId; }

private:
    std::size_t m_ConstId;
};

struct SettingsRecord final {
    SettingsContext SettingsContext;
    Settings        Settings;

    SettingsRecord(const SettingsRecord&)            = delete;
    SettingsRecord& operator=(const SettingsRecord&) = delete;

    SettingsRecord(SettingsRecord&&) noexcept            = default;
    SettingsRecord& operator=(SettingsRecord&&) noexcept = default;

    explicit SettingsRecord(struct SettingsContext&& SettingsContext,
                            struct Settings&&        Settings) noexcept
        : SettingsContext(std::move(SettingsContext))
        , Settings(std::move(Settings)) {}
};
