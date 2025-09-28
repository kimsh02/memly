#pragma once

#include <expected>
#include <optional>
#include <string>
#include <vector>

namespace Types {
using PathString         = std::string; // std::filesystem::path
using OptionalPathString = std::optional<PathString>;

using TimeString         = std::string; // std::chrono::system_clock::time_point
using OptionalTimeString = std::optional<TimeString>;

using IdVector = std::vector<std::size_t>;

template <typename FieldType>
    requires std::is_enum_v<FieldType>
struct ValidationError {
    FieldType   Field;
    std::string Message;

    ValidationError(const ValidationError&)            = delete;
    ValidationError& operator=(const ValidationError&) = delete;

    ValidationError(ValidationError&&) noexcept            = default;
    ValidationError& operator=(ValidationError&&) noexcept = delete;

    ValidationError(FieldType Field, std::string&& Message)
        : Field(Field)
        , Message(std::move(Message)) {}
};

template <typename FieldType>
using ValidationErrors = std::vector<ValidationError<FieldType>>;

template <typename FieldType>
using VResult = std::expected<void, ValidationErrors<FieldType>>;

template <typename FieldType>
inline std::string VResultToString(const VResult<FieldType>& Res) {
    std::string Out;

    const ValidationErrors<FieldType>& errors = Res.error();
    for (std::size_t I = 0; I < errors.size(); I++) {
        Out.append(errors.at(I).Message);
        if (I < errors.size() - 1)
            Out.append(", ");
    }
    return Out;
}
}
