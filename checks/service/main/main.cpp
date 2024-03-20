#include <userver/components/minimal_server_component_list.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include <uopenapi/all.hpp>

struct TestBody{
    std::int64_t left;
    std::int64_t right;
};

enum struct Operation{
    sum,
    div,
    sub,
    prod
};

namespace uopenapi::utils{
    template <>
    struct converter<std::string_view, Operation>{
        static Operation convert(std::string_view text){
            if (text == "+"){
                return Operation::sum;
            }
            else if (text == "-"){
                return Operation::sub;
            }
            else if (text == "/"){
                return Operation::div;
            }
            else if (text == "*"){
                return Operation::prod;
            }
            else {
                throw utils::formatted_exception("Unknowned value [{}] try to convert to Operation", text);
            }
        }

    };
}

namespace uopenapi::reflective{
    template <>
    struct schema_appender<Operation, none_requirements>{
        template <none_requirements>
        static void append(schema_view schemaView){
            place_ref_to_type<Operation>(schemaView.cur_place);
            auto type_node = schemaView.root["components"]["schemas"][schema_type_name<Operation>()];
            if (type_node.IsObject()){
                return;
            }
            type_node = userver::formats::yaml::Type::kObject;
            type_node["type"] = "string";
            auto enum_node = type_node["enum"];
            enum_node = userver::formats::yaml::Type::kArray;
            enum_node.PushBack("+");
            enum_node.PushBack("/");
            enum_node.PushBack("*");
            enum_node.PushBack("-");
        }
    };
}

struct TestRequest{
    TestBody body;
    std::optional<Operation> op;
};

struct ResponseBody{
    std::int64_t result;
};

struct Response{
    ResponseBody body;
};

using Resp200 = uopenapi::http::response<Response, 200>;

using Base = uopenapi::http::openapi_handler<TestRequest, Resp200>;
struct TestHandler : Base{
    static constexpr std::string_view kName = "test_handler";
    TestHandler(const userver::components::ComponentConfig& cfg,
                    const userver::components::ComponentContext& ctx)
            : Base(cfg, ctx)
    {}
    response handle(TestRequest req) const override{
        if (!req.op){
            req.op = Operation::sum;
        }
        std::int64_t result;
        switch (req.op.value()) {
            case Operation::sum:
                result = req.body.left + req.body.right;
                break;
            case Operation::div:
                result = req.body.left / req.body.right;
                break;
            case Operation::sub:
                result = req.body.left - req.body.right;
                break;
            case Operation::prod:
                result = req.body.left * req.body.right;
                break;
        }
        Resp200 resp200;
        resp200().body.result = result;
        return resp200;
    }
};

int main(int argc, char* argv[]) {
    auto component_list = userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .Append<userver::components::HttpClient>()
            .Append<userver::clients::dns::Component>()
            .Append<userver::server::handlers::TestsControl>();
    component_list.Append<TestHandler>();
    component_list.Append<uopenapi::http::openapi_descriptor>();

    return userver::utils::DaemonMain(argc, argv, component_list);
}