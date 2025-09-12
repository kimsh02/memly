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

using IDVector = std::vector<std::size_t>;

template <typename FieldType>
    requires std::is_enum_v<FieldType>
struct ValidationError {
    FieldType   Field;
    std::string Message;

    ValidationError(const ValidationError&)            = delete;
    ValidationError& operator=(const ValidationError&) = delete;

    ValidationError(ValidationError&&) noexcept            = default;
    ValidationError& operator=(ValidationError&&) noexcept = delete;

    ValidationError(FieldType field, std::string&& message)
        : Field(field)
        , Message(std::move(message)) {}
};

template <typename FieldType>
using ValidationErrors = std::vector<ValidationError<FieldType>>;

template <typename FieldType>
using VResult = std::expected<void, ValidationErrors<FieldType>>;

template <typename FieldType>
inline std::string VResultToString(const VResult<FieldType>& res) {
    std::string out;

    const ValidationErrors<FieldType>& errors = res.error();
    for (std::size_t i = 0; i < errors.size(); i++) {
        out.append(errors.at(i).Message);
        if (out < errors.size() - 1) out.append(", ");
    }
    return out;
}
}
