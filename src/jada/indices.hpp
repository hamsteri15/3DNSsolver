#pragma once
#include <range/v3/view/indices.hpp>

/// @brief Wrapper around ranges::indices
/// @param begin integer type begin index
/// @param end interger type end index
/// @return one-dimensional view of indices [begin, end)
static constexpr auto indices(auto&& begin, auto&& end){
    return ranges::views::indices(begin, end);
}


