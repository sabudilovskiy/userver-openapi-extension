#include "string_requirements.hpp"
#include <regex>

void uopenapi::reflective::check_pattern(std::string_view str, std::string_view pattern) {
    if (!std::regex_match(str.begin(), str.end(),  std::regex{pattern.begin(), pattern.end()})){
        throw utils::formated_exception("str: {}, doesn't match with: {}", str, pattern);
    }

}
