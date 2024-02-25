#include "format_datetime.hpp"
#include <userver/utils/datetime.hpp>

namespace uopenapi::reflective{
    void string_validator<kDateTime>::validate(std::string_view sv) {
        userver::utils::datetime::GuessLocalTimezoneStringtime(std::string{sv});
    }
}