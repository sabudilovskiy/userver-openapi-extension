Requirements are an abstraction that expresses the constraints imposed on a type or field. 
They affect schema generation, as well as validation during parsing/serialization.

In general, the entire system is designed in such a way as to support both requirements whose values are completely constexpr and those that are not.

Note that using constexpr requirements will allow the compiler to generate unique validation functions 
if it does not consider it necessary to inline the call, which in turn will reduce the number of unnecessary checks.

### Array Requirements

<details>
<summary>Declaration of array_requirements</summary>

```c++
struct array_requirements {
    utils::ce::optional<std::size_t> min_items;
    utils::ce::optional<std::size_t> max_items;
    bool unique_items = false;
};
```
</details>

The fields correspond to the corresponding restrictions on values from OpenApi.
* `min_items` Minimum size of array(inclusive).
* `max_items` Maximum size of array(inclusive).
* `unique_items` Prohibit duplicate items


### Number requirements

<details>
<summary>Declaration of number_requirements</summary>

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

The fields correspond to the corresponding restrictions on values from OpenApi.
* `minimum` Minimum of value. 
* `maximum` Maximum of value.
* `exclusive_minimum` Do not include the minimum in the acceptable range
* `exclusive_maximum` Do not include the maximum in the acceptable range
* `multiple_of` The value must be completely divisible by this number (the concept is completely applicable to fractional numbers, but they should be handled with extreme caution)

### String requirements

<details>
<summary>Declaration of number_requirements</summary>

```c++
template <utils::ce::string Format = "">
struct string_requirements {
    utils::ce::optional<std::size_t> min_length;
    utils::ce::optional<std::size_t> max_length;
    utils::ce::string pattern;
    static constexpr auto format = Format;
};
```
</details>

* `min_length` Minimum length of string(exclusive)
* `max_length` Maximum length of string(exclusive)
* `pattern` The regular expression(ECMA standard) that the value must match
* `Format` The name of the format that the value should satisfy

<details>
<summary>Declaration of string_validator</summary>

```c++
template <utils::ce::string Format = "">
template <utils::ce::string Format>
struct string_validator {
    // must have static validate_result validate(std::string_view)
};
```
</details>

<details>
<summary>Declaration of noop validator</summary>

```c++
template <>
struct string_validator<""> {
    static validate_result validate(std::string_view) {
        return validate_result::ok();
    }
};

```
</details>

The format check is performed as follows: called `string_validator<Format>::validate(str)`

validate must return type which satisfy `is_validate_result<T>`

Supported formats:

* `date-time`

