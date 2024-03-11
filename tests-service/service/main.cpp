#include <userver/components/minimal_server_component_list.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include <uopenapi/all.hpp>

struct TestBody{
    std::string first;
    std::string second;
};

struct TestRequest{
    std::vector<std::string> filter_name;
    std::string token;
    std::string test_cookie;
    TestBody body;
};

struct Empty{};
using Resp200 = uopenapi::http::response<Empty, 200>;

UOPENAPI_SOURCE_TYPE(TestRequest, filter_name, query);
UOPENAPI_SOURCE_TYPE(TestRequest, token, header);
UOPENAPI_SOURCE_TYPE(TestRequest, test_cookie, cookie);

using Base = uopenapi::http::openapi_handler<TestRequest, Resp200>;
struct TestHandler : Base{
    static constexpr std::string_view kName = "test_handler";
    TestHandler(const userver::components::ComponentConfig& cfg,
                    const userver::components::ComponentContext& ctx)
            : Base(cfg, ctx)
    {}
    response handle(TestRequest req) const override{
        LOG_INFO() << "filter_name: " << req.filter_name;
        LOG_INFO() << "token: " << req.token;
        LOG_INFO() << "body.first: " << req.body.first;
        LOG_INFO() << "body.second: " << req.body.second;
        return Resp200{};
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

    return userver::utils::DaemonMain(argc, argv, component_list);
}