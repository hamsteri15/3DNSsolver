#pragma once

#include "topaz/include/all.hpp"
#include "field.hpp"
#include "common/scalar_field.hpp"
#include "common/vector_field.hpp"





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