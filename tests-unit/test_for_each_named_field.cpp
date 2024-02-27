#include <userver/utest/utest.hpp>
#include <uopenapi/pfr_extension/for_each_named_field.hpp>
#include <uopenapi/json/reflective_parse.hpp>
#include <uopenapi/json/reflectie_serialize.hpp>
#include <userver/formats/parse/common_containers.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <uopenapi/reflective/requirements/requirements.hpp>
#include <uopenapi/reflective/requirements/string/format_datetime.hpp>
#include <uopenapi/reflective/requirements/integer/integer_requirements.hpp>
#include <uopenapi/pfr_extension/get_index.hpp>
#include <uopenapi/pfr_extension/sorted_names.hpp>
#include <uopenapi/pfr_extension/visit_all.hpp>

namespace {
    struct SecretStruct{
        std::string c;
        int a; //min: 1, max: 2
        std::string b;
    };
    struct SecretStruct2{
        std::string d;
        int a; //min: 1, max: 2
        std::string b;
    };
}

REQUIREMENTS_CE_UOPENAPI(SecretStruct, a) = uopenapi::reflective::integer_requirements<int>{
    .maximum = 4,
    .exclusive_minimum = true
};

REQUIREMENTS_CE_UOPENAPI(SecretStruct, b) = uopenapi::reflective::string_requirements<kDateTime>{};

UTEST(Openapi_json_Parse, SomeStruct){
    userver::formats::json::ValueBuilder json{userver::formats::common::Type::kObject};
    json["a"] = 5;
    json["b"] = "2024-02-25T10:50:44Z";
    json["c"] = "f";
    auto j = json.ExtractValue();
    auto secret_struct = j.As<SecretStruct>();
    ASSERT_EQ (secret_struct.a,1);
    ASSERT_EQ (secret_struct.b,"2002.");
    ASSERT_EQ (secret_struct.c, "f");
}

UTEST(Openapi_json_Serialize, SomeStruct){
    SecretStruct some_struct;
    some_struct.a = 5;
    some_struct.b = "2024-02-25T10:50:44Z";
    some_struct.c = "f";
    auto json = userver::formats::json::ValueBuilder{some_struct}.ExtractValue();
    ASSERT_EQ(json["a"].As<int>(), 5);
    ASSERT_EQ(json["b"].As<std::string>(), "2024-02-25T10:50:44Z");
    ASSERT_EQ(json["c"].As<std::optional<std::string>>(), "f");
}

struct VisitTwoStructures{
    std::string buf;

    template<uopenapi::utils::ce::string name>
    void operator()(auto& left, uopenapi::pfr_extension::place_holder){
        buf.append(fmt::format("name:{} left has: {}\n", name.AsStringView(), left));
    }
    template<uopenapi::utils::ce::string name>
    void operator()(uopenapi::pfr_extension::place_holder, auto& right){
        buf.append(fmt::format("name:{} right has: {}\n", name.AsStringView(), right));
    }

    template<uopenapi::utils::ce::string name>
    void operator()(auto&left, auto& right){
        buf.append(fmt::format("name:{} left has:{}, right has: {}\n", name.AsStringView(), left, right));
    }
};

struct AutoCastVisiter{
    template<uopenapi::utils::ce::string name>
    void operator()(auto& from, uopenapi::pfr_extension::place_holder){}

    template<uopenapi::utils::ce::string name>
    void operator()(uopenapi::pfr_extension::place_holder, auto& to){
        static_assert(sizeof(name) == 0, "Какое-то из полей отсуствует в объекте, который пытаются сконвертировать");
    }
    template<uopenapi::utils::ce::string name>
    void operator()(auto&from, auto& to){
        to = from;
    }
};

template <typename Value>
struct GoInterface{
    template <typename T>
    GoInterface(T&& t){
        uopenapi::pfr_extension::visit_all(AutoCastVisiter{}, t, value);
    }
    Value value;
};

struct Point {
    double x;
    double y;
    double mrv;
};

struct SomeShit{
    int y;
    int x;
    std::string message = "dsds";
};

void foo(GoInterface<Point> point){
}

UTEST(Openapi_json_Serialize, visit_two_structures){
    SomeShit shit{
        .y = 4,
        .x = 3,
        .message = "dsdsds"
    };
    foo(shit);
}

