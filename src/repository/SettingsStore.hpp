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

    [[nodiscard]] const Settings* Read() const noexcept { return &m_SettingsRecord.Settings; }

    enum class UserField { Name };

    using UVResult = Types::VResult<UserField>;

    [[nodiscard]] UVResult Update(Settings&& settings);

    explicit SettingsStore(const DatabaseQt& db)
        : m_Db(db)
        , m_SettingsRecord(initSettingsRecord()) {}

private:
    struct Default {
        inline static constexpr std::size_t s_TargetLangIdx = 9;
        inline static constexpr std::size_t s_UserID        = 1;
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

    SettingsRecord m_SettingsRecord;

    SettingsRecord initSettingsRecord();

    SettingsRecord dbRead();

    enum class AppField { LangIdx };
    enum class ContextField { ID };

    using AVResult = Types::VResult<AppField>;
    using CVResult = Types::VResult<ContextField>;

    // void validate(const SettingsContext&, const AppSettings&, const UserSettings&) const;
    [[nodiscard]] CVResult validateContextFields(const SettingsContext& settingsContext) const;
    [[nodiscard]] AVResult validateAppFields(const AppSettings& appSettings) const;
    [[nodiscard]] UVResult validateUserFields(const UserSettings& userSettings) const;
};
