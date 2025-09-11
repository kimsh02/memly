#include "repository/SettingsStore.hpp"

#include "util/Util.hpp"

SettingsRecord SettingsStore::dbRead() {
    m_ReadStmt.Bind(Default::s_UserID);
    m_ReadStmt.Exec();
    if (!m_ReadStmt.Next()) { Fatal("ReadStmt.Exec() did not return a row"); }
    SettingsContext settingsContext(m_ReadStmt.Value(0).toULongLong());
    AppSettings     appSettings(m_ReadStmt.Value(1).toULongLong());
    UserSettings    userSettings(m_ReadStmt.Value(2).toString().toStdString());
    m_ReadStmt.Finish();
    Settings       settings(std::move(appSettings), std::move(userSettings));
    SettingsRecord settingsRecord(std::move(settingsContext), std::move(settings));
    if (auto res = validateSystemFields(settingsRecord); !res) {
        Fatal(Types::VResultToString(res));
    }
    if (auto res = validateUserFields(settingsRecord.Settings); !res) {
        Fatal(Types::VResultToString(res));
    }
    return settingsRecord;
}

SettingsRecord SettingsStore::initSettingsRecord() {
    DatabaseQt::Stmt upsertStmt = m_Db.Prepare(SQL::s_UpsertSQL);
    upsertStmt.Bind(Default::s_UserID, Default::s_TargetLangIdx, std::string{});
    upsertStmt.Exec();
    return dbRead();
}

[[nodiscard]] SettingsStore::SVResult
SettingsStore::validateSystemFields(const SettingsRecord& settingsRecord) const {
    Types::ValidationErrors<SystemField> v;
    if (std::size_t id = settingsRecord.SettingsContext.ID(); id != Default::s_UserID) {
        v.emplace_back(SystemField::ID, std::format("Invalid user id: {}", id));
    }
    if (std::size_t idx = settingsRecord.Settings.AppSettings.LastTargetLanguageIndex;
        idx < 0 || idx >= Limit::s_LANG_COUNT) {
        v.emplace_back(SystemField::LangIdx, std::format("Invalid language index: {}", idx));
    }
    if (v.empty()) return SVResult{};
    return std::unexpected(std::move(v));
}

[[nodiscard]] SettingsStore::UVResult
SettingsStore::validateUserFields(const Settings& settings) const {
    Types::ValidationErrors<UserField> v;
    if (settings.UserSettings.Name.size() > Limit::s_MAX_NAME_LEN) {
        v.emplace_back(UserField::Name,
                       std::format("Name exceeds {} characters", Limit::s_MAX_NAME_LEN));
    }
    if (v.empty()) return UVResult{};
    return std::unexpected(std::move(v));
}

[[nodiscard]] SettingsStore::UVResult SettingsStore::Update(Settings&& settings) {
    if (auto res = validateUserFields(settings); !res) { return res; }
    m_UpdateStmt.Bind(settings.AppSettings.LastTargetLanguageIndex,
                      settings.UserSettings.Name,
                      Default::s_UserID);
    m_UpdateStmt.Exec();
    m_UpdateStmt.Finish();
    m_SettingsRecord = dbRead();
    return UVResult{};
}
