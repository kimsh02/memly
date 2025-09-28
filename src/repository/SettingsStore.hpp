#pragma once

#include "common/Types.hpp"
#include "database_qt/DatabaseQt.hpp"
#include "database_qt/SettingsRecord.hpp"
#include "services/LanguageTranslator.hpp"

class SettingsStore final {
public:
    SettingsStore(const SettingsStore&)            = delete;
    SettingsStore& operator=(const SettingsStore&) = delete;

    SettingsStore(SettingsStore&&) noexcept            = delete;
    SettingsStore& operator=(SettingsStore&&) noexcept = delete;

    [[nodiscard]] const Settings* Read() const noexcept {
        return &m_SettingsCache.at(Default::s_UserId).Settings;
    }



    using UserVResult = Types::VResult<UserField>;

    [[nodiscard]] UserVResult Update(Settings&& Settings);

    explicit SettingsStore(const DatabaseQt& Db);

private:
    struct Default {
        inline static constexpr std::size_t s_TargetLangIdx = 9;
        inline static constexpr std::size_t s_UserId        = 1;
    };

    struct SQL {
        inline static constexpr auto s_ReadSQL   = R"(
            SELECT * FROM settings WHERE id = ? LIMIT 1;)";
        inline static constexpr auto s_UpdateSQL = R"(
            UPDATE settings
            SET target_lang_idx = ?, name = ?
            WHERE id = ?;)";
        inline static constexpr auto s_UpsertSQL = R"(
            INSERT INTO settings(id, target_lang_idx, name)
            VALUES(?, ?, ?)
            ON CONFLICT(id) DO NOTHING;)";
    };

    struct Limit {
        inline static constexpr std::size_t s_LANG_COUNT   = LanguageTranslator::LangCount();
        inline static constexpr std::size_t s_MAX_NAME_LEN = 80;
    };

    const DatabaseQt& m_Db;

    DatabaseQt::Stmt m_ReadStmt   = m_Db.Prepare(SQL::s_ReadSQL);
    DatabaseQt::Stmt m_UpdateStmt = m_Db.Prepare(SQL::s_UpdateSQL);

    std::unordered_map<std::size_t, SettingsRecord> m_SettingsCache;

    void UpsertCache();

    enum class AppField { LangIdx };
    enum class ContextField { Id };

    using AppVResult     = Types::VResult<AppField>;
    using ContextVResult = Types::VResult<ContextField>;

    [[nodiscard]] ContextVResult
                              ValidateContextFields(const SettingsContext& SettingsContext) const;
    [[nodiscard]] AppVResult  ValidateAppFields(const AppSettings& AppSettings) const;
    [[nodiscard]] UserVResult ValidateUserFields(const UserSettings& UserSettings) const;
};
