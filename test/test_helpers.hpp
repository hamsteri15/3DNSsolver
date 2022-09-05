#pragma once

#include "common/mdspan.hpp"

template<size_t Dir, size_t N, class T>
void set_linear(span<T, N> s){

    auto indices = all_indices(s);
    for (auto idx : indices){
        auto ii = std::get<Dir>(idx);
        s(idx) = T(ii);
    }

}
