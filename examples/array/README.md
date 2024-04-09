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
