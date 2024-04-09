## Responses from server

This example uses arrays and the requirements for them. What and how to do it is described in the server configs and in main.cpp.

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
