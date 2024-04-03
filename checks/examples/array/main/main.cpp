#include <uopenapi/all.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

using namespace uopenapi;
using namespace uopenapi::reflective;

struct Body {
    std::optional<std::vector<std::int64_t>> first;
    std::optional<std::vector<std::int64_t>> second;
    std::optional<std::vector<std::int64_t>> thirst;
};

REQUIREMENTS_CE_UOPENAPI(Body, first) = array_requirements{.min_items = 2};

REQUIREMENTS_CE_UOPENAPI(Body, second) = array_requirements{.max_items = 2};

REQUIREMENTS_CE_UOPENAPI(Body,
                         thirst) = array_requirements{.unique_items = true};

struct Request {
    Body body;
    int index_add;
    std::int64_t value_add;
};

REQUIREMENTS_CE_UOPENAPI(Request, index_add) = number_requirements<int>{
    .minimum = 1, .maximum = 3};

struct Response {
    Body body;
};

using Resp200 = uopenapi::http::response<Response, 200>;

using Base = uopenapi::http::openapi_handler<Request, Resp200>;
struct Handler : Base {
    static constexpr std::string_view kName = "test-handler";
    Handler(const userver::components::ComponentConfig& cfg,
            const userver::components::ComponentContext& ctx)
        : Base(cfg, ctx) {}
    response handle(Request req) const override {
        Resp200 resp200;
        auto& resp_body = resp200->body;
        resp_body = req.body;
        auto& field = [&]() -> std::optional<std::vector<std::int64_t>>& {
            switch (req.index_add) {
                case 1:
                    return resp_body.first;
                case 2:
                    return resp_body.second;
                case 3:
                    return resp_body.thirst;
                default:
                    throw std::runtime_error("invariant");
            }
        }();
        if (!field) {
            field.emplace();
        }
        field->push_back(req.value_add);
        return resp200;
    }
};

int main(int argc, char* argv[]) {
    auto component_list =
        userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .Append<userver::components::HttpClient>()
            .Append<userver::clients::dns::Component>()
            .Append<userver::server::handlers::TestsControl>();
    component_list.Append<Handler>();
    component_list.Append<uopenapi::http::openapi_descriptor>();

    return userver::utils::DaemonMain(argc, argv, component_list);
}
