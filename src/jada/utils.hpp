#pragma once

#include <array>
#include <tuple>

#include <stdexcept> //std::runtime_error
#include <string>


#ifdef NDEBUG
constexpr void runtime_assert([[maybe_unused]] bool condition, [[maybe_unused]] const char* msg) {}
#else
constexpr void runtime_assert(bool condition, const char* msg) {
    if (!condition) throw std::runtime_error(msg);
}
#endif



template <typename tuple_t> constexpr auto tuple_to_array(tuple_t&& tuple) {
    constexpr auto get_array = [](auto&&... x) {
        return std::array{std::forward<decltype(x)>(x)...};
    };
    return std::apply(get_array, std::forward<tuple_t>(tuple));
}

