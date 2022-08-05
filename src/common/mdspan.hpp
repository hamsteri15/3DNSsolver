#pragma once


#include <range/v3/view/indices.hpp>
#include <experimental/mdspan>
#include "common/math.hpp"
#include "common/index_type.hpp"
#include "common/runtime_assert.hpp"



namespace stdex = std::experimental;

using layout = stdex::layout_right;

template<size_t N>
using extents = stdex::dextents<size_t, N>;




template<class ElementType, size_t N>
using span_base = stdex::mdspan<ElementType, extents<N>, layout>;

template<typename tuple_t>
constexpr auto get_array_from_tuple(tuple_t&& tuple)
{
    constexpr auto get_array = [](auto&& ... x){ return std::array{std::forward<decltype(x)>(x) ... }; };
    return std::apply(get_array, std::forward<tuple_t>(tuple));
}


template<class ElementType, size_t N>
struct span : public span_base<ElementType, N>
{
    using parent = span_base<ElementType, N>;
    using parent::mdspan; //Inherit all constructors
    using parent::operator();

    template<class... Is>
    auto& operator()(ranges::common_tuple<Is...> tpl){
        return parent::operator()(get_array_from_tuple(tpl));
    }

    template<class... Is>
    const auto& operator()(ranges::common_tuple<Is...> tpl) const{
        return parent::operator()(get_array_from_tuple(tpl));
    }


};


auto extent_to_array(auto ext){

    static constexpr size_t N = decltype(ext)::rank();
    using idx_t = typename decltype(ext)::index_type;

    std::array<idx_t, N> ret{};
    for (size_t i = 0; i < N; ++i){
        ret[i] = ext.extent(i);
    }

    return ret;
}


template<class Extents>
bool extents_equal_size(size_t size, Extents dims){

    auto arr = extent_to_array(dims);
    using idx_t = typename decltype(arr)::value_type;
    auto mult = std::accumulate(arr.begin(), arr.end(), idx_t(1), std::multiplies{});
    return size_t(mult) == size;

}

template<class Field, class Extents>
static inline auto make_span(Field& field, Extents dims){
    using value_type = typename Field::value_type;
    static constexpr size_t N = Extents::rank();
    runtime_assert(extents_equal_size(field.size(), dims), "Dimension mismatch in make_span");
    return span<value_type, N>(field.data(), dims);
}

template<class Field, class Extents>
static inline auto make_span(const Field& field, Extents dims){
    using value_type = typename Field::value_type;
    static constexpr size_t N = Extents::rank();
    runtime_assert(extents_equal_size(field.size(), dims), "Dimension mismatch in make_span");
    return span<value_type, N>(field.data(), dims);
}


template<class Span>
static constexpr size_t rank(const Span& span){
    return span.rank();
}


template<size_t dir, index_type amount>
auto shift(auto idx){

    std::get<dir>(idx) += amount;
    return idx;
    /*
    auto cpy = idx;
    std::get<dir>(cpy) = std::get<N>(cpy) + amount;
    return cpy;
    */
}
