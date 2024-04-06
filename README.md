## UserverOpenapiExtension

This library is an extension for the [userver](userver.tech) library and its task is to provide opportunities for declarative description of endpoint: requests, responses and other things. 
The current API is extremely unstable and I cannot give great guarantees for its immutability, but I will not break it unnecessarily.

For introspection of types, it is used boost.pfr.name 
Accordingly, there are certain requirements for compilers and versions of the standard: c++20, clang 15, gcc-11.

The library aims in many ways to emulate OpenAPI and be compatible with it, so some abstractions are an attempt to embody some of its features.

### Basic Example

Let's say we want to make some kind of super stupid endpoint that accepts 
login and password in the request body and returns a token. 
So far, let's consider only the happy path.

#### Request

```c++
struct RequestBody{
    std::string login;
    std::string password;
};

struct Request{
    RequestBody body;
};
```

#### Response

```c++
struct ResponseBody{
    boost::uuid::uuid token;
};

struct Response{
    ResponseBody body;
};
```


#### Handler

In order to use the type as a response, you need to wrap it in uopenapi::http::response<T, code>

```c++
using resp200 = uopenapi::http::response<Response, 200>;
```

Now we can declare a handler for endpoint:

```c++
using base = uopenapi::http::openapi_handler<Request,Response200,Response400 >;
struct handler : base{
        static constexpr std::string_view kName = "login_handler";
        handler(const userver::components::ComponentConfig& cfg,
                const userver::components::ComponentContext& ctx) : base(cfg, ctx){}
        
        //you can return any response from declaration, but there's only one here
        response handle(views::login::Request req) const override{
            //some logic
        }
};
```

### Requirements


Requirements are the embodiment of type properties in OpenAPI. 
For example, the following restrictions can be imposed on arrays:

<details>
<summary>Array requirements</summary>

```c++
struct array_requirements {
    utils::ce::optional<std::size_t> min_items;
    utils::ce::optional<std::size_t> max_items;
    bool unique_items = false;
};
```

</details>

Constraints can be set externally on any type field 
and then they will be checked during parsing and serialization. 
Accordingly, in the context of request processing, 
non-compliance with restrictions in the request is a parsing error and 400, 
and non-compliance with restrictions in the response is 500.


