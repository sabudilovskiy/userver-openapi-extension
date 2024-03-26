#include <string>
#include <uopenapi/utils/name_type.hpp>
#include <userver/utest/utest.hpp>

namespace utils_tests::name_type{
    struct Tests{};
}

using utils_tests::name_type::Tests;

UTEST(UtilsTests, NameType) {
    auto test = uopenapi::utils::get_full_name_type<Tests>();
    EXPECT_EQ(test, "utils_tests::name_type::Tests");
}

UTEST(UtilsTests, TypeInfo) {
    auto info = uopenapi::utils::get_type_name_info<Tests>();
    EXPECT_EQ(info.name, "Tests");
    ASSERT_EQ(info.count_ns, 2);
    EXPECT_EQ(info.namespaces[0], "utils_tests");
    EXPECT_EQ(info.namespaces[1], "name_type");
}
