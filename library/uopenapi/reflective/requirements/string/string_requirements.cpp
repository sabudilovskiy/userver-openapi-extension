#include "string_requirements.hpp"
#include <regex>

void uopenapi::reflective::validate(std::string_view str, uopenapi::reflective::basic_string_requirements requirements) {
    if (requirements.min_length && str.size() < requirements.min_length){
        throw utils::formated_exception("str.size: {}, min_length: {}", str.size(), *requirements.max_length);
    }
    if (requirements.max_length && str.size() > requirements.max_length){
        throw utils::formated_exception("str.size: {}, min_length: {}", str.size(), *requirements.max_length);
    }
    if (requirements.pattern && !std::regex_match(str.begin(), str.end(),  std::regex{*requirements.pattern})){
       throw utils::formated_exception("str: {}, doesn't match with: {}", str, *requirements.pattern);
    }
}
