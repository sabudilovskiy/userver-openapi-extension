Requirements are an abstraction that expresses the constraints imposed on a type or field. 
They affect schema generation, as well as validation during parsing/serialization.

In general, the entire system is designed in such a way as to support both requirements whose values are completely constexpr and those that are not.

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

