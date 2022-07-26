#pragma once
#include <cstddef>
#include <initializer_list>
#include "common/math_vector_base.hpp"


template <class T, size_t L>
struct MathVector : public MathVectorBase<T, L, MathVector<T, L>> {


    inline constexpr MathVector() = default;

    constexpr MathVector(std::initializer_list<T> list) {

        if (list.size() > L){
            throw std::logic_error("Invalid paramenter count for vector");
        }
        std::move(list.begin(), list.end(), &m_storage[0]);
    }

    const T* get_ptr() const { return &m_storage[0]; }
    T* get_ptr() { return &m_storage[0]; }

private:

    T m_storage[L];
};
