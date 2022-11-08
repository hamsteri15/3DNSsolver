#pragma once

#include <array>
#include <cstddef>
#include "common/mdspan.hpp"
#include "common/loop.hpp"

template<class Derived>
struct TiledStencil{


    static constexpr size_t padding = Derived::padding;
    static constexpr size_t direction = Derived::direction;

};

template<class T>
static constexpr auto get_direction(const TiledStencil<T>& stencil){
    return stencil.direction;
}



template<size_t N, class T>
std::array<size_t, N> get_padding(const TiledStencil<T>& stencil){
    std::array<size_t, N> ret{};
    ret[get_direction(stencil)] = stencil.padding;
    return ret;
}

template<size_t N, class... Ts>
std::array<size_t, N> get_padding(std::tuple<Ts...> tpl){

    auto arr_tpl = 
    std::apply(
        [](auto&&... args)
        {
            return std::make_tuple(get_padding<N>(args)...);
        },
        tpl);
    

    std::array<size_t, N> current{};
    
    auto compare = [&](auto& arr){

        for (size_t i = 0; i < N; ++i){
            if (arr[i] > current[i]){
                current[i] = arr[i];
            }
        }

    };

    auto f2 = [=] (auto&&... args){
        (compare(args), ...);
    };

    std::apply(f2, arr_tpl);
    return current;
}


template<class Ext, class... Ts>
auto make_padded_extent(Ext extent, Ts... ops){

    static constexpr size_t N = Ext::rank();

    std::array<size_t, N> arr{};
    auto padding = get_padding<N>(std::make_tuple(ops...));

    for (size_t i = 0; i < N; ++i){
        arr[i] = extent.extent(i) + 2 * padding[i];
    }

    return Ext(arr);
}



template<class Ext, class... Ts>
auto make_indices(Ext extent, Ts... ops){

    static constexpr size_t N = Ext::rank();

    auto padding = get_padding<N>(std::make_tuple(ops...));

    std::array<size_t, N> begin = padding;
    std::array<size_t, N> end{};

    for (size_t i = 0; i < N; ++i){
        end[i] = begin[i] + extent.extent(i);
    }

    return md_indices(begin, end);
}
