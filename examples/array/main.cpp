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

/*
In this example, I want to demonstrate
how requirements can be used.
And so, how to use them?

We will declare the structure that we will use as the body
of both the request and the response,
and also write the requirements for its fields.

 */

struct Body {
    std::optional<std::vector<std::int64_t>> first;
    std::optional<std::vector<std::int64_t>> second;
    std::optional<std::vector<std::int64_t>> third;
};

/*
Let's put requirements on each field.
*/

UOPENAPI_CE_REQUIREMENTS(Body, first) = array_requirements{.min_items = 2};

UOPENAPI_CE_REQUIREMENTS(Body, second) = array_requirements{.max_items = 2};

UOPENAPI_CE_REQUIREMENTS(Body,
                         third) = array_requirements{.unique_items = true};

/*
Now let's declare the type of request.
*/

struct Request {
    Body body;
    std::int64_t index_add = 0;
    std::int64_t value_add = 0;
};

/*
By default, fields from a query with names other than body are parsed from
query, and with body it is obvious from where.

In general, our plan is elementary: we will add a number to the corresponding
field from the body. Thus, we will be able to see not only the behavior.

if validation failed at the request parsing stage,
but also if validation failed at the serialization stage.

Let's write the requirements for index_add, since it should be obvious from 1
to 3. For this we will use number_requirements.

*/

UOPENAPI_CE_REQUIREMENTS(Request,
                         index_add) = number_requirements<std::int64_t>{
    .minimum = 1, .maximum = 3};

struct Response {
    Body body;
};

/*
Let's now declare the response and the handler.
*/

using Resp200 = uopenapi::http::response<Response, 200>;

using Base = uopenapi::components::openapi_handler<Request, Resp200>;

struct Handler : Base {
    static constexpr std::string_view kName = "test-handler";
    Handler(const userver::components::ComponentConfig& cfg,
            const userver::components::ComponentContext& ctx)
        : Base(cfg, ctx) {}
    // Response with a small letter is a usage inside the base class, which is
    // std::variant of all responses.
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
                    return resp_body.third;
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
    // append schemas components
    component_list.Append<uopenapi::components::schema_storage>();
    component_list.Append<uopenapi::components::schema_http_distributor>();

    return userver::utils::DaemonMain(argc, argv, component_list);
}
