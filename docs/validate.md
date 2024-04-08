Validation is the verification of the requirements set for a type or field.
If the requirement is constexpr and nttp, it will be forwarded as nttp, if not, then as a regular runtime argument.

How do I define validation functions for my type of requirements?
You need to write an ADL validation function, for nttp you need to use utils::nttp_adl.


<details>
<summary>Declaration of noop validate with none_requirements(default requirements)</summary>

```c++
template <typename T, none_requirements req>
validate_result validate(const T&, utils::nttp_adl<none_requirements, req>) {
    return validate_result::ok();
}
```
</details>

<details>
<summary>Declaration of array validate with array_requirements</summary>

```c++
template <typename T, array_requirements req>
validate_result validate(const std::vector<T>& value,
                         utils::nttp_adl<array_requirements, req>) {
    if (req.min_items && *req.min_items > value.size()) {
        return validate_result::error(
            "array has size: [{}] less than min_items: [{}]", value.size(),
            *req.min_items);
    }
    if (req.max_items && *req.max_items < value.size()) {
        return validate_result::error(
            "array has size: [{}] greater than max_items: [{}]", value.size(),
            *req.max_items);
    }
    if constexpr (req.unique_items) {
        using comparator = utils::indirect_comparator<const T*>;
        using hash = utils::indirect_hash<const T*, std::hash<T>>;
        std::unordered_set<const T*, hash, comparator> hash_set;
        for (auto& item : value) {
            const T* ptr = std::addressof(item);
            auto [it, emplaced] = hash_set.emplace(ptr);
            if (!emplaced) {
                auto first_index =
                    std::find(value.begin(), value.end(), item) - value.begin();
                auto second_index = &item - value.data();
                return validate_result::error(
                    "array has non-unique items. first_index: [{}], "
                    "second_index: [{}]",
                    first_index, second_index);
            }
        }
    }
    return validate_result::ok();
}
```
</details>

The return type must satisfy the is_validate_result concept. In particular, my validate_result class satisfies it.
<details>
<summary>Concept</summary>

```c++
template <typename T>
concept is_validate_result = requires(T&& t) {
    { t ? 0 : 0 }; //check explicit bool conversion
    { t.error_message() }
    { t.has_error() } -> std::convertible_to<bool>;
};
```
</details>

<details>
<summary>Declaration of validate_result</summary>

```c++
struct validate_result {
    static validate_result error(std::exception& exc) {
        return validate_result{.has_error_ = true,
                               .error_message_ = exc.what()};
    }
    template <typename... T>
    static validate_result error(fmt::format_string<T...> str, T&&... args) {
        return validate_result{.has_error_ = true,
                               .error_message_ = fmt::format(
                                   std::move(str), std::forward<T>(args)...)};
    }
    static validate_result ok() { return validate_result{.has_error_ = false}; }
    operator bool() const { return !has_error_; }
    bool has_error() const { return has_error_; }
    auto&& error_message() & { return error_message_; }

    auto&& error_message() const& { return error_message_; }

    auto error_message() && { return std::move(error_message_); }

   public:
    bool has_error_ = false;
    std::string error_message_;
};
```
</details>

Validation of optional fields occurs when there is a value and, accordingly, with a validation call specifically for it, that is, it is not necessary to overload functions for them.
