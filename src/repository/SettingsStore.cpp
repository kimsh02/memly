#include "repository/SettingsStore.hpp"

SettingsRecord SettingsStore::initSettingsRecord() {
    DatabaseQt::Stmt upsertStmt =
        m_DatabaseQt.Prepare("INSERT INTO settings(id, target_language_index, name) "
                             "VALUES(?, ?, ?) "
                             "ON CONFLICT(id) DO NOTHING;");
    upsertStmt.Bind(s_UserID, s_DefaultTargetLanguageIndex, QString{}).Exec();

    m_ReadStmt.Exec();
    SettingsContext settingsContext(m_ReadStmt.Value(0).toULongLong());
    Settings        settings(AppSettings(m_ReadStmt.Value(1).toULongLong()),
                      UserSettings(m_ReadStmt.Value(2).toString().toStdString()));
    return SettingsRecord(std::move(settingsContext), std::move(settings));
}
