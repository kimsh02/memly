#include "repository/SettingsStore.hpp"

// #include <stdexcept>
#include "util/Util.hpp"

SettingsRecord SettingsStore::dbReadSettings() {
    m_ReadStmt.Bind(Default::s_UserID);
    m_ReadStmt.Exec();
    if (!m_ReadStmt.Next()) { Fatal("ReadStmt.Exec() did not return a row"); }
    SettingsContext settingsContext(m_ReadStmt.Value(0).toULongLong());
    AppSettings     appSettings(m_ReadStmt.Value(1).toULongLong());
    UserSettings    userSettings(m_ReadStmt.Value(2).toString().toStdString());
    m_ReadStmt.Finish();
    Settings       settings(std::move(appSettings), std::move(userSettings));
    SettingsRecord settingsRecord(std::move(settingsContext), std::move(settings));
    if (auto res = validateSystemFields(settingsRecord); !res) { Fatal(res.error()); }
    if (auto res = validateUserFields(settingsRecord.Settings); !res) {
        Fatal(res.error().at(0).Message);
    }
    return settingsRecord;
}

SettingsRecord SettingsStore::initSettingsRecord() {
    DatabaseQt::Stmt upsertStmt = m_DatabaseQt.Prepare(SQL::s_UpsertSettingsSQL);
    upsertStmt.Bind(Default::s_UserID, Default::s_TargetLangIdx, std::string{});
    upsertStmt.Exec();
    return dbReadSettings();
}

[[nodiscard]] SettingsStore::SVResult
SettingsStore::validateSystemFields(const SettingsRecord& settingsRecord) const {
    if (std::size_t id = settingsRecord.SettingsContext.ID(); id != Default::s_UserID) {
        return std::unexpected(std::format("Invalid user id: {}", id));
    }
    if (std::size_t idx = settingsRecord.Settings.AppSettings.LastTargetLanguageIndex;
        idx < 0 || idx >= Limit::s_LANG_COUNT) {
        return std::unexpected(std::format("Invalid language index: {}", idx));
    }
    return SVResult{};
}

[[nodiscard]] UVResult SettingsStore::validateUserFields(const Settings& settings) const {
    std::vector<SettingsValidationError> v;
    if (settings.UserSettings.Name.size() > Limit::s_MAX_NAME_LEN) {
        v.emplace_back(SettingsField::Name,
                       std::format("Name exceeds {} characters", Limit::s_MAX_NAME_LEN));
    }
    if (v.empty()) return UVResult{};
    return std::unexpected(std::move(v));
}

[[nodiscard]] UVResult SettingsStore::Update(Settings&& settings) {
    if (auto res = validateUserFields(settings); !res) { return res; }
    m_UpdateStmt.Bind(settings.AppSettings.LastTargetLanguageIndex,
                      settings.UserSettings.Name,
                      Default::s_UserID);
    m_UpdateStmt.Exec();
    m_UpdateStmt.Finish();
    return UVResult{};
}
