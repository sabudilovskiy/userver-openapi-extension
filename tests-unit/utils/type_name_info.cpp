#include <userver/utest/utest.hpp>
#include <string>

#include <uopenapi/utils/name_type.hpp>

inline namespace utils_tests{
    inline namespace name_type{
        struct Tests{

        };
    }
}

template <typename T>
constexpr auto test_get_name_function() {
    std::string_view raw_name = BOOST_CURRENT_FUNCTION;
    auto clear_name =
            uopenapi::utils::details::get_clear_name<sizeof("test_get_name_function")>(
                    raw_name);
    return clear_name;
}

UTEST(UtilsTests, NameType){
    auto test = test_get_name_function<Tests>();
    EXPECT_EQ(test, "utils_tests::name_type::Tests");
}