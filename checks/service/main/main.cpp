#include <uopenapi/all.hpp>
#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

struct OperationBody {
    std::int64_t left;
    std::int64_t right;
};

UOPENAPI_DECLARE_ENUM(Operation, int, sum, div, sub, prod );

struct OperationRequest {
    OperationBody body;
    std::optional<Operation> op;
};

struct ResponseBody {
    std::int64_t result;
};

struct Response {
    ResponseBody body;
};

using Resp200 = uopenapi::http::response<Response, 200>;

using Base = uopenapi::http::openapi_handler<OperationRequest, Resp200>;
struct OperationHandler : Base {
    static constexpr std::string_view kName = "operation-handler";
    OperationHandler(const userver::components::ComponentConfig& cfg,
                     const userver::components::ComponentContext& ctx)
        : Base(cfg, ctx) {}
    response handle(OperationRequest req) const override {
        if (!req.op) {
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
    auto component_list =
        userver::components::MinimalServerComponentList()
            .Append<userver::server::handlers::Ping>()
            .Append<userver::components::TestsuiteSupport>()
            .Append<userver::components::HttpClient>()
            .Append<userver::clients::dns::Component>()
            .Append<userver::server::handlers::TestsControl>();
    component_list.Append<OperationHandler>();
    component_list.Append<uopenapi::http::openapi_descriptor>();

    return userver::utils::DaemonMain(argc, argv, component_list);
}
