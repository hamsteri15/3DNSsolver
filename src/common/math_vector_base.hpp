#pragma once
#include <array>
#include <cstddef>
#include <initializer_list>
#include <algorithm>
#include <numeric>
#include <iostream>

//#include "utils/constexpr_functions.hpp"



template <class T, size_t L, class S> struct MathVectorBase {

    static_assert(L > 0, "Math vector needs to be at least size 1.");
    static_assert(std::is_arithmetic<T>::value, "MathVector elements need to be arithmetic type.");

    // clang-format off

    //CRTP injection of access to the data, these must be implemented by all derived classes
    constexpr const T* get_ptr() const { return static_cast<const S*>(this)->get_ptr(); }
    constexpr       T* get_ptr()       { return static_cast<      S*>(this)->get_ptr(); }

    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;



    template<class TT, size_t LL, class SS>
    friend std::ostream& operator<<(std::ostream& os, const MathVectorBase<TT,LL,SS>& v);


    inline static constexpr size_t             size() noexcept      {return L;}



    inline constexpr T*                 data()      noexcept       {return get_ptr();}
    inline constexpr const T*           data()      const noexcept {return get_ptr();}
    inline constexpr iterator         begin()     noexcept       {return iterator(get_ptr());}
    inline constexpr const_iterator   begin()     const noexcept {return const_iterator(get_ptr());}
    inline constexpr const_iterator   cbegin()    const noexcept {return const_iterator(get_ptr());}
    inline constexpr iterator         end()       noexcept       {return iterator(get_ptr() + L);}
    inline constexpr const_iterator   end()       const noexcept {return const_iterator(get_ptr() + L);}
    inline constexpr const_iterator   cend()      const noexcept {return const_iterator(get_ptr() + L);}

    inline constexpr reverse_iterator        rbegin()          noexcept {return reverse_iterator(end());}
    inline constexpr const_reverse_iterator   rbegin()    const noexcept {return const_reverse_iterator(end());}
    inline constexpr const_reverse_iterator   crbegin()   const noexcept {return const_reverse_iterator(end());}

    inline constexpr reverse_iterator        rend()            noexcept {return reverse_iterator(begin());}
    inline constexpr const_reverse_iterator   rend()      const noexcept {return const_reverse_iterator(begin());}
    inline constexpr reverse_iterator        crend()     const noexcept {return const_reverse_iterator(begin());}


    inline constexpr const T& operator[](size_t idx) const { return get_ptr()[idx]; }
    inline constexpr     T& operator[](size_t idx)       { return get_ptr()[idx]; }



    inline constexpr T max() const {return *std::max_element(cbegin(), cend());}
    inline constexpr T min() const {return *std::min_element(cbegin(), cend());}
    inline constexpr T elementwise_product() const { return std::accumulate(begin(), end(), T(1), std::multiplies{});}
    inline constexpr T elementwise_sum() const { return std::accumulate(begin(), end(), T(0));}




    inline constexpr S abs() const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = constexpr_abs(tp[i]);
        return r;
    }

    inline constexpr bool non_zero() const {
        const T* tp = get_ptr();
        for (size_t i = 0; i < L; i++) {
            if (tp[i] != T(0)) {return true;}
        }
        return false;
    }

    inline constexpr bool all_zero() const {
        return !(this->non_zero());
    }


    template<size_t I>
    inline constexpr T get() const {return get_ptr()[I];}



    //set()
    inline constexpr void set(const T& a) {
        T* tp = get_ptr();
        for (size_t i = 0; i < L; i++) { tp[i] = a; };
    }
    inline constexpr void set(const T* ptr) {
        T* tp = get_ptr();
        for (size_t i = 0; i < L; i++) { tp[i] = ptr[i]; };
    }

    template <class V> inline constexpr void set(const MathVectorBase<T, L, V>& v){ set(v.get_ptr()); }


    inline constexpr void set_zero() { set(0); }

    //positivate
    inline constexpr S operator+() const { return *this; }

    //negate
    inline constexpr S operator-() const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = -tp[i];
        return r;
    }

    // ================= scalar operators + - * / %
    inline constexpr S operator+(const T& a) const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] + a;
        return r;
    }
    inline constexpr S operator-(const T& a) const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] - a;
        return r;
    }
    inline constexpr S operator*(const T& a) const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] * a;
        return r;
    }
    inline constexpr S operator/(const T& a) const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] / a;
        return r;
    }
    inline constexpr S operator%(const T& a) const {
        const T* tp = get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] % a;
        return r;
    }

    // ================= scalar operators += -= *= /= %=
    inline constexpr S& operator=(const T& a)  { set(a);            return *static_cast<S*>(this); }
    inline constexpr S& operator+=(const T& a) { set(operator+(a)); return *static_cast<S*>(this); }
    inline constexpr S& operator-=(const T& a) { set(operator-(a)); return *static_cast<S*>(this); }
    inline constexpr S& operator*=(const T& a) { set(operator*(a)); return *static_cast<S*>(this); }
    inline constexpr S& operator/=(const T& a) { set(operator/(a)); return *static_cast<S*>(this); }
    inline constexpr S& operator%=(const T& a) { set(operator%(a)); return *static_cast<S*>(this); }


    // ================= vector operators += -= *= /= %=
    template <class V> inline constexpr S& operator=(const MathVectorBase<T, L, V>& v)  {set(v);            return *static_cast<S*>(this); }
    template <class V> inline constexpr S& operator+=(const MathVectorBase<T, L, V>& v) {set(operator+(v)); return *static_cast<S*>(this); }
    template <class V> inline constexpr S& operator-=(const MathVectorBase<T, L, V>& v) {set(operator-(v)); return *static_cast<S*>(this); }
    template <class V> inline constexpr S& operator*=(const MathVectorBase<T, L, V>& v) {set(operator*(v)); return *static_cast<S*>(this); }
    template <class V> inline constexpr S& operator/=(const MathVectorBase<T, L, V>& v) {set(operator/(v)); return *static_cast<S*>(this); }
    template <class V> inline constexpr S& operator%=(const MathVectorBase<T, L, V>& v) {set(operator%(v)); return *static_cast<S*>(this); }

    // ================= vector operators + - * / %
    template <class V>
    inline constexpr S operator+(const MathVectorBase<T, L, V>& v) const {
        const T* tp = get_ptr(); const T* vp = v.get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] + vp[i];
        return r;
    }

    template <class V>
    inline constexpr S operator-(const MathVectorBase<T, L, V>& v) const {
        const T* tp = get_ptr(); const T* vp = v.get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] - vp[i];
        return r;
    }

    template <class V>
    inline constexpr S operator*(const MathVectorBase<T, L, V>& v) const {
        const T* tp = get_ptr(); const T* vp = v.get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] * vp[i];
        return r;
    }

    template <class V>
    inline constexpr S operator/(const MathVectorBase<T, L, V>& v) const {
        const T* tp = get_ptr(); const T* vp = v.get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] / vp[i];
        return r;
    }

    template <class V>
    inline constexpr S operator%(const MathVectorBase<T, L, V>& v) const {
        const T* tp = get_ptr(); const T* vp = v.get_ptr(); S r; T* rp = r.get_ptr();
        for (size_t i = 0; i < L; i++) rp[i] = tp[i] % vp[i];
        return r;
    }

    // ================= vector comparison operators == and !=
    template <class V>
    inline constexpr bool operator==(const MathVectorBase<T, L, V>& v) const {
        const T* tp = get_ptr(); const T* vp = v.get_ptr();
        for (size_t i = 0; i < L; i++) if (tp[i] != vp[i]) return false;
        return true;
    }

    template <class V>
    inline constexpr bool operator!=(const MathVectorBase<T, L, V>& v) const { return (!operator==(v)); }

    // clang-format on
};

template<class T, size_t L, class S>
static constexpr S operator+(const T& lhs, const MathVectorBase<T,L,S>& rhs){
    return rhs + lhs;
}

template<class T, size_t L, class S>
static constexpr S operator-(const T& lhs, const MathVectorBase<T,L,S>& rhs){
    return -rhs + lhs;
}

template<class T, size_t L, class S>
static constexpr S operator*(const T& lhs, const MathVectorBase<T,L,S>& rhs){
    return rhs * lhs;
}

template<class T, size_t L, class S>
static constexpr S operator/(const T& lhs, const MathVectorBase<T,L,S>& rhs){
    S r;
    for (size_t i = 0; i < L; ++i){
        r[i] = lhs / rhs[i];
    }
    return r;
}



template<class T, size_t L, class S>
std::ostream& operator<<(std::ostream& os, const MathVectorBase<T,L,S>& v) {

    os << "{";
    for (size_t i = 0; i < L; ++i){
        os << v[i] << " ";
    }
    os << "}";
    return os;

}

template<class T, size_t L, class S>
static constexpr S elementwise_max(const MathVectorBase<T,L,S>& lhs, const MathVectorBase<T,L,S>& rhs) {

    S r;
    for (size_t i = 0; i < L; ++i){
        r[i] = std::max(lhs[i], rhs[i]);
    }
    return r;
}

template<class T, size_t L, class S>
static constexpr S elementwise_max(const MathVectorBase<T,L,S>& lhs, T rhs) {

    S r;
    for (size_t i = 0; i < L; ++i){
        r[i] = std::max(lhs[i], rhs);
    }
    return r;
}

template<class T, size_t L, class S>
static constexpr S elementwise_max(T lhs, const MathVectorBase<T,L,S>& rhs) {

    return elementwise_max(rhs, lhs);
}

template<class T, size_t L, class S>
static constexpr S elementwise_min(const MathVectorBase<T,L,S>& lhs, const MathVectorBase<T,L,S>& rhs) {

    S r;
    for (size_t i = 0; i < L; ++i){
        r[i] = std::min(lhs[i], rhs[i]);
    }
    return r;
}

template<class T, size_t L, class S>
static constexpr S elementwise_min(const MathVectorBase<T,L,S>& lhs, T rhs) {

    S r;
    for (size_t i = 0; i < L; ++i){
        r[i] = std::min(lhs[i], rhs);
    }
    return r;
}

template<class T, size_t L, class S>
static constexpr S elementwise_min(T lhs, const MathVectorBase<T,L,S>& rhs) {

    return elementwise_min(rhs, lhs);
}


template<class T, size_t L, class S>
static constexpr S abs(const MathVectorBase<T,L,S>& v) {
    return v.abs();
}

template<class T, size_t L, class S>
static constexpr auto max(const MathVectorBase<T,L,S>& v) {
    return v.max();
}

template<class T, size_t L, class S>
static constexpr auto min(const MathVectorBase<T,L,S>& v) {
    return v.min();
}

template<class T, size_t L, class S>
static constexpr auto elementwise_sum(const MathVectorBase<T,L,S>& v) {
    return v.elementwise_sum();
}

template<class T, size_t L, class S>
static constexpr size_t nonzero_element_count(const MathVectorBase<T,L,S>& v) {

    size_t count = 0;
    for (const auto& e : v) {
        if (e != T(0)) {count++;}
    }
    return count;
    //No idea why this isnt working
    //auto count = std::count_if(v.begin(), v.end(), T(0));
    //return size_t(count);

}


template<class T, size_t L, class S>
static constexpr T dot(const MathVectorBase<T,L,S>& lhs, const MathVectorBase<T,L,S>& rhs) {

    return std::inner_product(lhs.begin(), lhs.end(), rhs.begin(), T(0));

}



template<class T, size_t L, class S>
static constexpr T mag(const MathVectorBase<T,L,S>& v) {
    return sqrt(dot(v, v));
}

template<class T, size_t L, class S>
static constexpr T all_positive(const MathVectorBase<T,L,S>& v) {

    for (const auto& e : v){
        if (e < T(0)) {
            return false;
        }
    }
    return true;
}
