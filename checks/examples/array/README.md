### Example with requirements

In this example, I want to demonstrate
how requirements can be used using the example of array requirements. 
And so, how to use them?

We will declare the structure that we will use as the body
of both the request and the response, 
and also write the requirements for its fields.

```c++
struct Body {
    std::optional<std::vector<std::int64_t>> first;
    std::optional<std::vector<std::int64_t>> second;
    std::optional<std::vector<std::int64_t>> third;
};
```

Let's put requirements on each field.

```c++
REQUIREMENTS_CE_UOPENAPI(Body, first) = array_requirements{.min_items = 2};

REQUIREMENTS_CE_UOPENAPI(Body, second) = array_requirements{.max_items = 2};

REQUIREMENTS_CE_UOPENAPI(Body,
                         third) = array_requirements{.unique_items = true};
```

<details>
<summary>Specify requirements for field without macro</summary>

```c++
template <> inline constexpr auto ::uopenapi::reflective::requirements_field< Body, "first"> = array_requirements{.min_items = 2};
```
</details>

On the right, we use a structure that expresses requirements for array. 
This is a simple aggregate type, except that you have to use a special type for constexpr optional.

<details>
<summary>Declaration of array_requirements</summary>

```c++
struct array_requirements {
    utils::ce::optional<std::size_t> min_items;
    utils::ce::optional<std::size_t> max_items;
    bool unique_items = false;
};
```

The fields correspond to the corresponding restrictions on values from OpenApi.
* `min_items` Minimum size of array(inclusive).
* `max_items` Maximum size of array(inclusive).
* `unique_items` Prohibit duplicate items

</details>

Now let's declare the type of request. 

```c++
struct Request {
    Body body;
    int index_add = 0;
    std::int64_t value_add = 0;
};
```

By default, fields from a query with names other than body are parsed from query, 
and with body it is obvious from where.

In general, our plan is elementary: we will add a number to the corresponding field from the body. 
Thus, we will be able to see not only the behavior 
if validation failed at the request parsing stage, 
but also if validation failed at the serialization stage.

Let's write the requirements for index_add, since it should be obvious from 1 to 3. 
For this we will use number_requirements.

<details>
<summary>Definition of number_requirements</summary>

```c++
template <typename T>
struct number_requirements {
    utils::ce::optional<T> minimum;
    utils::ce::optional<T> maximum;
    bool exclusive_minimum = false;
    bool exclusive_maximum = false;
    utils::ce::optional<T> multiple_of;
};
```
</details>

```c++
REQUIREMENTS_CE_UOPENAPI(Request, index_add) = number_requirements<int>{
    .minimum = 1, .maximum = 3};
```

Let's now declare the response and the handler.

```c++
struct Response {
    Body body;
};

using Resp200 = uopenapi::http::response<Response, 200>;

using Base = uopenapi::http::openapi_handler<Request, Resp200>;

struct Handler : Base {
    static constexpr std::string_view kName = "test-handler";
    Handler(const userver::components::ComponentConfig& cfg,
            const userver::components::ComponentContext& ctx)
        : Base(cfg, ctx) {
    }

    response handle(Request req) const override {
        
    }
};

```

Response with a small letter is a usage inside the base class, which is std::variant of all responses.

Therefore, you can do this.

```c++
response handle(Request req) const override {
    Resp200 resp200;
    auto& resp_body = resp200->body;
    resp_body = req.body;
    return resp200;
}
```



Ok, let's now send a request to the server and look at the response.

<details>
<summary>Empty body and empty queries</summary>

```json
{
	"message": "Some error happens where server tried to parse request: [Not founded queries with name: [index_add]]"
}
```
</details>

<details>
<summary>String in index_add</summary>

```json
{
  "message": "Some error happens where server tried to parse request: [utils::FromString error: \"no number found\" while converting \"adaa\" to int]"
}
```
</details>

<details>
<summary>Violation of the requirements for index_add</summary>

```json
{
  "message": "Some error happens where server tried to parse request: [Failed validate query with name: [index_add], error: [value: [0] less than minumum: [1]]]"
}
```
</details>

<details>
<summary>Wrong type of first</summary>

```json
{
  "message": "Some error happens where server tried to parse request: [Field 'first[0]' is of a wrong type. Expected: intValue, actual: stringValue]"
}
```
</details>

<details>
<summary>Violation of the requirements for first</summary>

```json
{
  "message": "Some error happens where server tried to parse request: [An error occurred while parsing the field on path: [/]. Field name: [first], message: [array has size: [1] less than min_items: [2]]]"
}
```
</details>

<details>
<summary>Violation of the requirements for second</summary>

```json
{
  "message": "Some error happens where server tried to parse request: [An error occurred while parsing the field on path: [/]. Field name: [second], message: [array has size: [3] greater than max_items: [2]]]"
}
```
</details>

<details>
<summary>Violation of the requirements for third</summary>

```json
{
  "message": "Some error happens where server tried to parse request: [An error occurred while parsing the field on path: [/]. Field name: [third], message: [array has non-unique items. first_index: [0], second_index: [1]]]"
}
```
</details>

Fairly clear and high-quality error messages. 
The status code 400 is natural for all cases. 
Okay, let's write a simple logic that will use the fields from the query in order to modify the body of the request (and the response, respectively).

<details>
<summary>Added code</summary>

```c++
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
```
</details>

Now we will send a request with an empty body, 
index_add 1 and value_add 0. 
Then, due to the fact that the number of elements in the returned body in the first field will be less than min_items, validation will fail.

<details>
<summary>Response</summary>

```json
{
	"message": "service unavailable"
}
```
</details>

<details>
<summary>Message in log</summary>

```
Unexpected error from serialize: An error occurred while serializing the field. Field name: first, message: array has size: [1] less than min_items: [2]
```
</details>
