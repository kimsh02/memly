#pragma once

#include <qsqlquery.h>

#include "database_qt/DatabaseQt.hpp"
#include "database_qt/SettingsRecord.hpp"

class SettingsStore final {
public:
    SettingsStore(const SettingsStore&)            = delete;
    SettingsStore& operator=(const SettingsStore&) = delete;

    SettingsStore(SettingsStore&&) noexcept            = delete;
    SettingsStore& operator=(SettingsStore&&) noexcept = delete;

    [[nodiscard]] const Settings* Read() const noexcept { return &m_SettingsRecord.Settings; }

    [[nodiscard]] bool Update(Settings&& settings);

    explicit SettingsStore(const DatabaseQt& databaseQt) noexcept
        : m_DatabaseQt(databaseQt)
        , m_SettingsRecord(initSettingsRecord()) {}

private:
    static constexpr std::size_t s_DefaultTargetLanguageIndex = 9;
    static constexpr std::size_t s_UserID                     = 1;

    static constexpr auto s_ReadSettingsSQL = R"(
        SELECT * FROM settings WHERE id = 1 LIMIT 1;)";

    static constexpr auto s_UpdateSettingsSQL = R"(
        UPDATE settings
        SET language = ?, theme = ?, notifications = ?
        WHERE id = 1;)";

    const DatabaseQt& m_DatabaseQt;

    DatabaseQt::Stmt m_ReadStmt   = m_DatabaseQt.Prepare(s_ReadSettingsSQL);
    DatabaseQt::Stmt m_UpdateStmt = m_DatabaseQt.Prepare(s_UpdateSettingsSQL);

    SettingsRecord m_SettingsRecord;

    SettingsRecord initSettingsRecord();
};
