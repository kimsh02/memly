#include "repository/SettingsStore.hpp"

#include "util/Util.hpp"

void SettingsStore::UpsertCache() {
    if (m_SettingsCache.size() > 1) {
        Fatal("More than one SettingsRecord was cached in m_SettingsCache");
    }
    m_ReadStmt.Bind(Default::s_UserId);
    m_ReadStmt.Exec();
    if (!m_ReadStmt.Next()) {
        Fatal(std::format("Could not read expected settings row id: {}", Default::s_UserId));
    }
    SettingsContext SettingsContext(m_ReadStmt.Value(0).toULongLong());
    if (auto Res = ValidateContextFields(SettingsContext); !Res) {
        Fatal(Types::VResultToString(Res));
    }
    AppSettings AppSettings(m_ReadStmt.Value(1).toULongLong());
    if (auto Res = ValidateAppFields(AppSettings); !Res) {
        Fatal(Types::VResultToString(Res));
    }
    UserSettings UserSettings(m_ReadStmt.Value(2).toString().toStdString());
    m_ReadStmt.Finish();
    if (auto Res = ValidateUserFields(UserSettings); !Res) {
        Fatal(Types::VResultToString(Res));
    }
    Settings       Settings(std::move(AppSettings), std::move(UserSettings));
    SettingsRecord SettingsRecord(std::move(SettingsContext), std::move(Settings));
    m_SettingsCache.insert_or_assign(Default::s_UserId, std::move(SettingsRecord));
}

SettingsStore::SettingsStore(const DatabaseQt& Db)
    : m_Db(Db) {
    DatabaseQt::Stmt UpsertStmt = m_Db.Prepare(SQL::s_UpsertSQL);
    UpsertStmt.Bind(Default::s_UserId, Default::s_TargetLangIdx, std::string{});
    UpsertStmt.Exec();

    UpsertCache();
}

[[nodiscard]] SettingsStore::CVResult
SettingsStore::ValidateContextFields(const SettingsContext& SettingsContext) const {
    Types::ValidationErrors<ContextField> VE;
    if (std::size_t Id = SettingsContext.Id(); Id != Default::s_UserId) {
        VE.emplace_back(ContextField::Id, std::format("Invalid user id: {}", Id));
    }
    if (VE.empty())
        return CVResult{};
    return std::unexpected(std::move(VE));
}

[[nodiscard]] SettingsStore::AVResult
SettingsStore::ValidateAppFields(const AppSettings& AppSettings) const {
    Types::ValidationErrors<AppField> VE;
    if (std::size_t Idx = AppSettings.LastTargetLanguageIndex;
        Idx < 0 || Idx >= Limit::s_LANG_COUNT) {
        VE.emplace_back(AppField::LangIdx, std::format("Invalid language index: {}", Idx));
    }
    if (VE.empty())
        return AVResult{};
    return std::unexpected(std::move(VE));
}

[[nodiscard]] SettingsStore::UVResult
SettingsStore::ValidateUserFields(const UserSettings& UserSettings) const {
    Types::ValidationErrors<UserField> VE;
    if (const auto& S = UserSettings.Name; S.size() > Limit::s_MAX_NAME_LEN) {
        VE.emplace_back(
            UserField::Name,
            std::format("Settings name exceeds {} characters: {}", Limit::s_MAX_NAME_LEN, S));
    }
    if (VE.empty())
        return UVResult{};
    return std::unexpected(std::move(VE));
}

[[nodiscard]] SettingsStore::UVResult SettingsStore::Update(Settings&& Settings) {
    if (auto Res = ValidateUserFields(Settings.UserSettings); !Res) {
        return Res;
    }
    m_UpdateStmt.Bind(Settings.AppSettings.LastTargetLanguageIndex,
                      Settings.UserSettings.Name,
                      Default::s_UserId);
    m_UpdateStmt.Exec();
    m_UpdateStmt.Finish();

    UpsertCache();
    return UVResult{};
}
