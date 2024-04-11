The schema of server is currently being generated in OpenAPI format in yaml.

Schema generation occurs recursively using schema_appender specializations. 

<details>
<summary>Declaration of schema_appender</summary>

```c++
template <typename T, typename requirements>
struct schema_appender {
    // can have static void append(schema_view, requirements)
    // can have static void append<requirements>(schema_view)
};
```
</details>

schema_view and schema are types that simplify writing appenders. 
I will not give their implementation here, you can look at the source code.

But I'll give you an example of the schema_appender implementation for std::string.

```c++
template <utils::ce::string format>
struct schema_appender<std::string, string_requirements<format>> {
    template <string_requirements<format> req>
    static void append(schema_view schema) {
        userver::formats::yaml::ValueBuilder& cur = schema.cur_place;
        cur["type"] = "string";
        if (!format.empty()) {
            cur["format"] = format.AsString();
        }
        if (req.min_length) {
            cur["minLength"] = *req.min_length;
        }
        if (req.max_length) {
            cur["maxLength"] = *req.max_length;
        }
        if (!req.pattern.empty()) {
            cur["pattern"] = req.pattern.AsString();
        }
    }
};

template <>
struct schema_appender<std::string, none_requirements> {
    template <none_requirements>
    static void append(schema_view schema) {
        userver::formats::yaml::ValueBuilder& cur = schema.cur_place;
        cur["type"] = "string";
    }
};
```

The type names for aggregates work as follows: the namespaces are written in the same order, 
but :: replaced with a dot. After that, the type name will be written.
If you want to change the type name for the schema, you can overload the function:
```c++
std::string uopenapi::reflective::schema_type_name<T>
```

In order for the scheme to be exported, 
you need to attach `uopenapi::components::schema_storage` and `uopenapi::components::schema_http_distributor`.

The first component stores the schema, the second is a regular http handler, 
which will export the schema using the path specified in the config.



```yaml
schema-storage:
    #required to OAS 3.0.0 schema
    servers:
        - description: stable
          url: top_secret
    load-enabled: true
    #required to OAS 3.0.0 schema
    info:
        title: Some Server Doc
        description: Some server
        version: 1.0.0
schema-http-distributor:
    method: GET
    #generated schema can be get on this path
    path: /openapi
    task_processor: main-task-processor
```
