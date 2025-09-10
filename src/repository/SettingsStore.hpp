#pragma once

#include <expected>

#include "database_qt/DatabaseQt.hpp"
#include "database_qt/SettingsRecord.hpp"
#include "services/LanguageTranslator.hpp"

enum class SettingsField { Name };

struct SettingsValidationError {
    SettingsField Field;
    std::string   Message;

    SettingsValidationError(const SettingsValidationError&)            = delete;
    SettingsValidationError& operator=(const SettingsValidationError&) = delete;

    SettingsValidationError(SettingsValidationError&&) noexcept            = default;
    SettingsValidationError& operator=(SettingsValidationError&&) noexcept = delete;

    SettingsValidationError(SettingsField field, std::string&& message)
        : Field(field)
        , Message(std::move(message)) {}
};

using UVResult = std::expected<void, std::vector<SettingsValidationError>>;

class SettingsStore final {
public:
    SettingsStore(const SettingsStore&)            = delete;
    SettingsStore& operator=(const SettingsStore&) = delete;

    SettingsStore(SettingsStore&&) noexcept            = delete;
    SettingsStore& operator=(SettingsStore&&) noexcept = delete;

    [[nodiscard]] const Settings* Read() {
        m_SettingsRecord = dbReadSettings();
        return &m_SettingsRecord.Settings;
    }

    [[nodiscard]] UVResult Update(Settings&& settings);

    explicit SettingsStore(const DatabaseQt& databaseQt) noexcept
        : m_DatabaseQt(databaseQt)
        , m_SettingsRecord(initSettingsRecord()) {}

private:
    struct Default {
        inline static constexpr std::size_t s_TargetLangIdx = 9;
        inline static constexpr std::size_t s_UserID        = 1;
    };

    struct SQL {
        inline static constexpr auto s_ReadSettingsSQL = R"(
            SELECT * FROM settings WHERE id = ? LIMIT 1;)";

        inline static constexpr auto s_UpdateSettingsSQL = R"(
            UPDATE settings
            SET target_lang_idx = ?, name = ?
            WHERE id = ?;)";

        inline static constexpr auto s_UpsertSettingsSQL = R"(
            INSERT INTO settings(id, target_lang_idx, name)
            VALUES(?, ?, ?)
            ON CONFLICT(id) DO NOTHING;)";
    };

    struct Limit {
        inline static constexpr std::size_t s_LANG_COUNT   = LanguageTranslator::LangCount();
        inline static constexpr std::size_t s_MAX_NAME_LEN = 80;
    };

    const DatabaseQt& m_DatabaseQt;

    DatabaseQt::Stmt m_ReadStmt   = m_DatabaseQt.Prepare(SQL::s_ReadSettingsSQL);
    DatabaseQt::Stmt m_UpdateStmt = m_DatabaseQt.Prepare(SQL::s_UpdateSettingsSQL);

    SettingsRecord m_SettingsRecord;

    SettingsRecord initSettingsRecord();

    SettingsRecord dbReadSettings();

    using SVResult = std::expected<void, std::string>;

    [[nodiscard]] SVResult validateSystemFields(const SettingsRecord& settingsRecord) const;
    [[nodiscard]] UVResult validateUserFields(const Settings& settings) const;
};
