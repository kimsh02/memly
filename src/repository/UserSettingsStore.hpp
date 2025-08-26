#pragma once

class UserSettingsStore final {
public:
    UserSettingsStore(const UserSettingsStore&)            = delete;
    UserSettingsStore& operator=(const UserSettingsStore&) = delete;

    UserSettingsStore(UserSettingsStore&&) noexcept            = delete;
    UserSettingsStore& operator=(UserSettingsStore&&) noexcept = delete;
};
