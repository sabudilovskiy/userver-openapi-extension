#include "string_requirements.hpp"
#include <regex>

bool uopenapi::reflective::check_pattern(std::string_view str, std::string_view pattern) {
    return std::regex_match(str.begin(), str.end(),  std::regex{pattern.begin(), pattern.end()});
}
