#pragma once

#include <range/v3/view/cartesian_product.hpp>

template<class... Rngs>
constexpr auto cartesian_product(Rngs &&... rngs){
    return ranges::views::cartesian_product(rngs...);
}

