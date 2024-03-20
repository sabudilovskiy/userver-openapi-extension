#pragma once
#include <string>

namespace uopenapi::tests_only{
    namespace details
    {
        inline std::string drop_first_and_last_symbol(std::string_view text)
        {
            return std::string{text.substr(1, text.size() - 2)};
        }
    }  // namespace impl

#define UOPENAPI_RAW_STRING(X) ::uopenapi::tests_only::details::drop_first_and_last_symbol(X)
}