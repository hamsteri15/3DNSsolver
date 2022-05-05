#pragma once

#include <array>

#include "common/allocator.hpp"
#include "common/scalar.hpp"
#include "common/scalar_field.hpp"
#include "common/expression.hpp"

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
///should be avoided. Instead, use functions get_components() / set_components(f) 
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
    template<Expression_c Expr>
    vectorField(const Expr& expr) : parent(expr) {} 

    ///
    ///@brief Returns the number of vector elements in the field
    ///
    ///@return size_t number of vector elements
    ///
    size_t size() const {return size_t(this->chunk_size());}


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
    auto get_components(size_t i) const {this->get_chunk(i);}


    auto get_all_components() {return this->get_all_chunks();}
    auto get_all_components() const {return this->get_all_chunks();}


    ///
    ///@brief Given a range of values, sets the i-th component of the individual vector elements
    ///
    ///@tparam Range 
    ///@param i the component index
    ///@param f the values to set as the components
    ///
    template<class Range>
    void set_components(size_t i, const Range& f) {set_chunk(i, f);} 


};

/*
namespace std {
    template <typename T, typename U>
    struct tuple_size<boost::tuples::cons<T, U>>
        : boost::tuples::length<boost::tuples::cons<T, U>>
    { };

    template <size_t I, typename T, typename U>
    struct tuple_element<I, boost::tuples::cons<T, U>>
        : boost::tuples::element<I, boost::tuples::cons<T, U>>
    { };
}

template<class Tuple>
decltype(auto) sum_components(Tuple const& tuple) {
  auto sum_them = [](auto const&... e)->decltype(auto) {
    return (e+...);
  };
  return std::apply( sum_them, tuple );
};
*/

template <size_t N> scalarField dot(const vectorField<N>& lhs, const vectorField<N>& rhs) {

    (void) lhs;
    (void) rhs;    
    /*
    vectorField<N> tmp = lhs * rhs;

    auto begins = tmp.zipped_begin();
    auto ends = tmp.zipped_end();

    
    auto sum = [](auto tpl) {
        (void) tpl;
    };

    std::transform(begins, ends, result.begin(), sum);
    */    
    
    scalarField result(lhs.size(), 0);

    return result;

    //auto tpl = tmp.get_all_components();
    //return scalarField(lhs.size(), 0);



    /*
    auto sum = [](auto const&... e) -> decltype(auto){
        return (e+...);
    };
    auto expr = boost::detail::apply(sum, tpl);

    //auto result = sum_components(tpl);
    (void) expr;
    //auto array = make_array(tpl);
    */


    //return scalarField(result);
    /*
    return scalarField(10, 1);

    auto tpl = get_all_chunks()
    */





    /*
    using iterator = decltype(mult.begin());

    std::array<iterator, N> begins;
    std::array<iterator, N> ends;
    for (size_t i = 0; i < N; ++i){
        begins[i] = mult.begin() + i * lhs.size()
    }
    */

    /*

    scalarField ret(mult.size(), 0);

    for (size_t i = 0; i < N; ++i){
        ret = ret + mult.get_components(i);
    }
    */
    //return ret;
}
