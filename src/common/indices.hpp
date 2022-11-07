#pragma once
#include <range/v3/view/indices.hpp>


static constexpr auto indices(auto&& begin, auto&& end){
    return ranges::views::indices(begin, end);
}
