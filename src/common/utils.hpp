#pragma once

#include <array>
#include <tuple>

template <typename tuple_t> constexpr auto get_array_from_tuple(tuple_t&& tuple) {
    constexpr auto get_array = [](auto&&... x) {
        return std::array{std::forward<decltype(x)>(x)...};
    };
    return std::apply(get_array, std::forward<tuple_t>(tuple));
}