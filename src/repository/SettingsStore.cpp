#include "repository/SettingsStore.hpp"

#include "util/Util.hpp"

SettingsRecord SettingsStore::dbRead() {
    m_ReadStmt.Bind(Default::s_UserID);
    m_ReadStmt.Exec();
    if (!m_ReadStmt.Next()) {
        Fatal("ReadStmt.Exec() did not return a row");
    }
    SettingsContext settingsContext(m_ReadStmt.Value(0).toULongLong());
    if (auto res = validateContextFields(settingsContext); !res) {
        Fatal(Types::VResultToString(res));
    }
    AppSettings appSettings(m_ReadStmt.Value(1).toULongLong());
    if (auto res = validateAppFields(appSettings); !res) {
        Fatal(Types::VResultToString(res));
    }
    UserSettings userSettings(m_ReadStmt.Value(2).toString().toStdString());
    m_ReadStmt.Finish();
    if (auto res = validateUserFields(userSettings); !res) {
        Fatal(Types::VResultToString(res));
    }
    Settings       settings(std::move(appSettings), std::move(userSettings));
    SettingsRecord settingsRecord(std::move(settingsContext), std::move(settings));
    return settingsRecord;
}

SettingsRecord SettingsStore::initSettingsRecord() {
    DatabaseQt::Stmt upsertStmt = m_Db.Prepare(SQL::s_UpsertSQL);
    upsertStmt.Bind(Default::s_UserID, Default::s_TargetLangIdx, std::string{});
    upsertStmt.ExecImmediate();
    return dbRead();
}

[[nodiscard]] SettingsStore::CVResult
SettingsStore::validateContextFields(const SettingsContext& settingsContext) const {
    Types::ValidationErrors<ContextField> v;
    if (std::size_t id = settingsContext.ID(); id != Default::s_UserID) {
        v.emplace_back(ContextField::ID, std::format("Invalid user id: {}", id));
    }
    if (v.empty())
        return CVResult{};
    return std::unexpected(std::move(v));
}

[[nodiscard]] SettingsStore::AVResult
SettingsStore::validateAppFields(const AppSettings& appSettings) const {
    Types::ValidationErrors<AppField> v;
    if (std::size_t idx = appSettings.LastTargetLanguageIndex;
        idx < 0 || idx >= Limit::s_LANG_COUNT) {
        v.emplace_back(AppField::LangIdx, std::format("Invalid language index: {}", idx));
    }
    if (v.empty())
        return AVResult{};
    return std::unexpected(std::move(v));
}

[[nodiscard]] SettingsStore::UVResult
SettingsStore::validateUserFields(const UserSettings& userSettings) const {
    Types::ValidationErrors<UserField> v;
    if (const auto& s = userSettings.Name; s.size() > Limit::s_MAX_NAME_LEN) {
        v.emplace_back(
            UserField::Name,
            std::format("Settings name exceeds {} characters: {}", Limit::s_MAX_NAME_LEN, s));
    }
    if (v.empty())
        return UVResult{};
    return std::unexpected(std::move(v));
}

[[nodiscard]] SettingsStore::UVResult SettingsStore::Update(Settings&& settings) {
    if (auto res = validateUserFields(settings.UserSettings); !res) {
        return res;
    }
    m_UpdateStmt.Bind(settings.AppSettings.LastTargetLanguageIndex,
                      settings.UserSettings.Name,
                      Default::s_UserID);
    m_UpdateStmt.ExecImmediate();
    m_UpdateStmt.Finish();

    m_SettingsRecord = dbRead();
    return UVResult{};
}
