The meaning of the requirements_field variable is in the specification of constraints for a specific type.
Please note that using the appropriate requirements is reasonable only for a limited number of types that are supported. You can read about their types and other things here.

<details>
<summary>Declaration</summary>


```c++
namespace uopenapi::reflective {
    template <typename T, utils::ce::string name>
    constexpr auto requirements_field = none_requirements{};
}

```
</details>

<details>
<summary>Usage with macro</summary>

```c++
struct Body {
    std::optional<std::vector<std::int64_t>> first;
};

REQUIREMENTS_CE_UOPENAPI(Body, first) = array_requirements{.min_items = 2};
```
</details>

<details>
<summary>Usage without macro</summary>

```c++
struct Body {
    std::optional<std::vector<std::int64_t>> first;
};

template <> inline constexpr auto ::uopenapi::reflective::requirements_field< Body, "first"> = array_requirements{.min_items = 2};
```
</details>

The forward value of this variable in the validation and schema generation function occurs depending on whether it is constexpr and is nttp.
If yes, then as nttp, if not, then as a regular runtime argument.

