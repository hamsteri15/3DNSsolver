#pragma once

#include <array>

#include "common/allocator.hpp"
#include "common/expression.hpp"
#include "common/scalar.hpp"
#include "common/scalar_field.hpp"

#include "topaz/include/all.hpp"

template <typename tuple_t> constexpr auto make_array(tuple_t&& tuple) {
    constexpr auto get_array = [](auto&&... x) {
        return std::array{std::forward<decltype(x)>(x)...};
    };
    return std::apply(get_array, std::forward<tuple_t>(tuple));
}

///
///@brief A field of N-dimensional vectors where the storage is allocated as a single large chunk
///(Struct-of-arrays-style). Although indexing is supported, assigning in a elementwise manner
/// should be avoided. Instead, use functions get_components() / set_components(f)
///
///@tparam N dimension of each individual vector
///
template <size_t N> struct vectorField : public topaz::NumericSoa<N, scalar, scalar_allocator_t> {

private:
    using parent = topaz::NumericSoa<N, scalar, scalar_allocator_t>;

public:
    vectorField() = default;

    ///
    ///@brief Construct a new vectorField from pack of scalarFields.
    ///       Example: a vectorField<3> could be constructed from the array of vector components
    ///       vectorField<3>(x, y, z)
    ///
    ///@tparam Fields
    ///@param fields the arrays of vector components (N in total)
    ///
    template <scalarField_c... Fields>
    explicit vectorField(const Fields&... fields)
        : parent(make_array(std::make_tuple(fields...))) {}

    ///
    ///@brief Construct from an expression type to allow operator oveloads to be evaluated lazily
    ///
    ///@tparam Expr
    ///@param expr A range expression
    ///
    template <Expression_c Expr>
    vectorField(const Expr& expr)
        : parent(expr) {}

    ///
    ///@brief Returns the number of vector elements in the field
    ///
    ///@return size_t number of vector elements
    ///
    size_t size() const { return size_t(this->chunk_size()); }

    ///
    ///@brief Modifyable view to all i-th components of the individual vector elements
    ///
    ///@param i the component index
    ///@return range of components
    ///
    auto get_components(size_t i) { this->get_chunk(i); }

    ///
    ///@brief Non-modifyable view to all i-th components of the individual vector elements
    ///
    ///@param i the component index
    ///@return range of components
    ///
    auto get_components(size_t i) const { this->get_chunk(i); }

    auto get_all_components() { return this->get_all_chunks(); }
    auto get_all_components() const { return this->get_all_chunks(); }

    ///
    ///@brief Given a range of values, sets the i-th component of the individual vector elements
    ///
    ///@tparam Range
    ///@param i the component index
    ///@param f the values to set as the components
    ///
    template <class Range> void set_components(size_t i, const Range& f) { set_chunk(i, f); }
};


template <size_t N> auto dot(const vectorField<N>& lhs, const vectorField<N>& rhs) {

    const auto tmp = lhs * rhs;

    auto chunk_sum = [](const auto&... e){return (e + ...);};

    return topaz::chunked_reduce<N>(tmp, chunk_sum);
}
