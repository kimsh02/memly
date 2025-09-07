#include "repository/SettingsStore.hpp"

#include <stdexcept>

SettingsRecord SettingsStore::dbReadSettings() {
    m_ReadStmt.Bind(Default::s_UserID);
    m_ReadStmt.Exec();
    if (!m_ReadStmt.Next()) { throw std::runtime_error("Settings read did not return a row"); }
    SettingsContext settingsContext(m_ReadStmt.Value(0).toULongLong());
    AppSettings     appSettings(m_ReadStmt.Value(1).toULongLong());
    UserSettings    userSettings(m_ReadStmt.Value(2).toString().toStdString());
    m_ReadStmt.Finish();
    Settings       settings(std::move(appSettings), std::move(userSettings));
    SettingsRecord settingsRecord(std::move(settingsContext), std::move(settings));
    validateSystemFields(settingsRecord);
    if (!validateUserFields(settingsRecord.Settings)) {
        std::runtime_error("Invalid user fields from DB read");
    }
    return settingsRecord;
}

SettingsRecord SettingsStore::initSettingsRecord() {
    DatabaseQt::Stmt upsertStmt = m_DatabaseQt.Prepare(SQL::s_UpsertSettingsSQL);
    upsertStmt.Bind(Default::s_UserID, Default::s_TargetLangIdx, QString{});
    upsertStmt.Exec();
    return dbReadSettings();
}

void SettingsStore::validateSystemFields(SettingsRecord& settingsRecord) {
    if (std::size_t id = settingsRecord.SettingsContext.ID(); id != Default::s_UserID) {
        throw std::runtime_error(std::format("Invalid user id: {}", id));
    }
    if (std::size_t idx = settingsRecord.Settings.AppSettings.LastTargetLanguageIndex;
        idx < 0 || idx >= Limit::s_LANG_COUNT) {
        throw std::runtime_error(std::format("Invalid language index: {}", idx));
    }
}

[[nodiscard]] VResult SettingsStore::validateUserFields(Settings& settings) const {
    std::vector<SettingsValidationError> v;
    if (settings.UserSettings.Name.size() > Limit::s_MAX_NAME_LEN) {
        v.emplace_back(SettingsField::Name,
                       std::format("Name exceeds {} characters", Limit::s_MAX_NAME_LEN).c_str());
    }
    if (v.empty()) return {};
    return std::unexpected(v);
}

[[nodiscard]] VResult SettingsStore::Update(Settings&& settings) {
    if (auto res = validateUserFields(settings); !res) { return res; }
    m_UpdateStmt.Bind(settings.AppSettings.LastTargetLanguageIndex,
                      settings.UserSettings.Name,
                      Default::s_UserID);
    m_UpdateStmt.Exec();
    m_UpdateStmt.Finish();
    m_SettingsRecord = dbReadSettings();
    return {};
}
