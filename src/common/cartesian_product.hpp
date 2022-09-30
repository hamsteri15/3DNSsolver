#pragma once

#include <range/v3/view/indices.hpp>
#include <range/v3/view/cartesian_product.hpp>

//TODO: maybe std::forward
template<class... Rngs>
constexpr auto cartesian_product(Rngs &&... rngs){
    return ranges::views::cartesian_product(rngs...);
}

/*
template<class... Rngs>
struct CartProdReturnType{



    using type = decltype(cartesian_product<Rngs...>())

};
*/
