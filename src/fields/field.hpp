#pragma once

#include <concepts>
#include <topaz.hpp>
#include "fields/allocator.hpp"
#include "math/vector.hpp"
#include "math/scalar.hpp"

template<class ET>
using Field = topaz::NumericArray<ET, allocator_t<ET>>;


template<class T>
concept field_c = topaz::IsRangeOrNumericArray_v<T>;

namespace topaz{


struct Mag {

    template <class T>
    inline CUDA_HOSTDEV auto operator()(const T& t) const
        -> decltype(mag(t)) {
        return mag(t);
    }
};

struct Dot {
    template <class T1, class T2>
    inline CUDA_HOSTDEV auto operator()(const T1& a, const T2& b) const
        -> decltype(dot(a,b)) {
        return dot(a,b);
    }
};


template <class T, typename = std::enable_if_t<IsRangeOrNumericArray_v<T>>>
inline CUDA_HOSTDEV auto mag(const T& t) {
    return transform(t, Mag{});
}

template<class T1, class T2>
requires field_c<T1> && field_c<T2>
inline constexpr CUDA_HOSTDEV auto dot(const T1& lhs, const T2& rhs){
    return smart_transform(lhs, rhs, Dot{});
}

template<size_t N, class T>
requires field_c<T>
inline constexpr CUDA_HOSTDEV auto dot(const T& lhs, const Vector<N>& rhs){
    auto rng = make_constant_range(rhs, lhs.size());
    return transform(lhs, rng, Dot{});
}



}

static auto serialize(const Field<scalar>& f){
    
    return std::vector<scalar>(f.begin(), f.end());
}

template<size_t L>
static auto serialize(const Field<Vector<L>>& f){

    size_t n = f.size() * L;

    std::vector<scalar> ret(n);

    for (size_t i = 0; i < f.size(); ++i){

        for (size_t j = 0; j < L; ++j){
            ret[i*L + j] = f[i][j];
        }
    }
    return ret;


}