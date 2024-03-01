#include "format_datetime.hpp"

#include <userver/utils/datetime.hpp>

namespace uopenapi::reflective {
validate_result string_validator<kDateTime>::validate(std::string_view sv) {
    try {
        userver::utils::datetime::GuessLocalTimezoneStringtime(std::string{sv});
        return validate_result::ok();
    } catch (std::exception& exc) {
        return validate_result::error(exc);
    }
}
}  // namespace uopenapi::reflective