#pragma once

#include "topaz/include/all.hpp"
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

    template <class T>
    inline CUDA_HOSTDEV auto operator()(const T& a, const T& b) const
        -> decltype(dot(a,b)) {
        return dot(a,b);
    }
};


template <class T, typename = std::enable_if_t<IsRangeOrNumericArray_v<T>>>
inline CUDA_HOSTDEV auto mag(const T& t) {
    return transform(t, Mag{});
}

template <class T1,
          class T2,
          typename = std::enable_if_t<SupportsBinaryExpression_v<T1, T2>>>
inline CUDA_HOSTDEV auto dot(const T1& lhs, const T2& rhs) {

    return smart_transform(lhs, rhs, Dot{});
}


}