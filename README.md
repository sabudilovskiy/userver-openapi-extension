# UserverOpenapiExtension

## Description

This library is an extension for the [userver](userver.tech) library and its task is to provide opportunities for declarative description of endpoint: requests, responses and other things. 
The current API is extremely unstable and I cannot give great guarantees for its immutability, but I will not break it unnecessarily.

For introspection of types, it is used boost.pfr.name 
Accordingly, there are certain requirements for compilers and versions of the standard: c++20, clang 15, gcc-11.

The library aims in many ways to emulate OpenAPI and be compatible with it, so some abstractions are an attempt to embody some of its features.

- [Basic Example](#basic-example)
- [Features](#features)
- [Examples](#examples)
- [Using with CMake](#using-with-cmake)
- [CMakeOptions](#cmake-options)

## Basic Example

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
using base = uopenapi::components::openapi_handler<Request,Response200,Response400 >;
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

## Features

- Imposing [requirements](docs/requirements.md) the outside on the [fields](docs/field%20requirements.md) of structures
- [Validate](docs/validate.md) fields of types
- Auto-generation of the [schema](docs/schema.md) of the entire service, at least for those endpoints that are created using the library.
- Supports basic OpenApi types: number, array, string, object.
- Supports header, cookie, query and json body.
- Auxiliary things for enams that allow you to generate their schemes, as well as parse them.
- Schema generation, validation, parsing and serialization are fully extensible, both due to new types and new requirements

## Full list of supporting by default c++ types

- `std::string`
- `boost::optional<T>`/`std::optional<T>` if T is supported
- `aggregate types`
- `userver::utils::datetime::Date`
- `std::is_arithmetic<T>`
- [`enum`](docs/enum.md)
- `boost::uuid::uuid`

## Full list of supporting by default openapi features

- `string`
  - `minLength`
  - `maxLength`
  - `format`
    - `date-time`
    - `uuid` (boost::uuid::uuid)
    - `date` (userver::utils::datetime::Date)
  - `pattern`
- `array`
  - `minimum`
  - `maximum`
  - `exclusiveMinimum`
  - `exclusiveMaximum`
  - `multipleOf`
- `number`
  - `minimum`
  - `maximum`
  - `exclusiveMinimum`
  - `exclusiveMaximum`
  - `multipleOf`
- `object`
  - `additionalProperties: false`

## Examples

- Enum [here](examples/enum)
- Array + array_requirements [here](examples/array)
- Test service [here](https://github.com/sabudilovskiy/uopenapi-service)

## Using with CMake

Fetch content: 
```CMake
include(FetchContent)
FetchContent_Declare(
  uopenapi
  GIT_REPOSITORY https://github.com/sabudilovskiy/userver-openapi-extension
  GIT_TAG "origin/trunk"
)
FetchContent_MakeAvailable(uopenapi)
target_link_libraries(service public openapi-extension-userver_library)
```

Subdirectory:

Clone this repository into folder with your project 2. Add these lines to it's CMakeLists.txt

```cmake
add_subdirectory(uopenapi)
target_link_libraries(service PUBLIC openapi-extension-userver_library)
```


## CMake Options

All options had predicate `openapi-extension-userver_`.
The options must be set before `FetchContent_MakeAvailable` or `add_subdirectory`.

Example of set: 

```cmake
set(openapi-extension-userver_BUILD_UNIT_TESTS false)
```

* `FIXED_STRING_SIZE` - The size of the `utils::ce::string`. The string cannot exceed this size. They are used to convey the names of parameters, within the requirements framework, and so on. Default: 64
* `CREATE_MACROS` - Whether the library will create macros. Default: `true`
* `DEFAULT_REFLECTIVABLE` - The standard meaning for the concept of reflexivity. If set to true, then all aggregates are considered reflective. Default: `true`
* `BUILD_UNIT_TESTS` - Whether to build unit tests of the library. By default: `true`
* `BUILD_TEST_SERVICES` - Whether to build examples of the library. By default: `true`
* `COMPILE_OPTIONS_VISIBLE` - Visibility of library compilation options. At the moment, there is only -Wno-missing-field-initializers to suppress unnecessary warnings for the lack of initialization of all fields in aggregates. Default: `PUBLIC`
