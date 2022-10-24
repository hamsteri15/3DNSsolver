#pragma once

#include "equation/volumetric_field.hpp"

template <class ET, size_t N> struct Flux {

    Flux() = default;

    Flux(const CartesianGrid<N>& grid, extents<N> padding)
        : m_f(grid, padding) {}

    // TODO: use concepts
    template <class Range_t> Flux& operator=(const Range_t& rng) {
        topaz::copy(rng, *this);
        return *this;
    }

    auto begin() { return m_f.begin(); }
    auto end() { return m_f.end(); }

    auto&       value() { return m_f; }
    const auto& value() const { return m_f; }

private:
    VolumetricField<ET, N> m_f;
};

template <class ET, size_t N> struct SplitFlux {

    SplitFlux() = default;

    SplitFlux(const CartesianGrid<N>& grid, extents<N> padding)
        : m_f_left(grid, padding)
        , m_f_right(grid, padding) {}

    // TODO: use concepts
    template <class Range_t> SplitFlux& operator=(const Range_t& rng) {
        topaz::copy(rng, *this);
        return *this;
    }

    auto&       left_value() { return m_f_left; }
    const auto& left_value() const { return m_f_left; }

    auto&       right_value() { return m_f_right; }
    const auto& right_value() const { return m_f_right; }

    auto begin() {
        auto tpl = topaz::adl_make_tuple(m_f_left.begin(), m_f_right.begin());
        return topaz::detail::make_zip_iterator(tpl);
    }
    auto begin() const {
        auto tpl = topaz::adl_make_tuple(m_f_left.begin(), m_f_right.begin());
        return topaz::detail::make_zip_iterator(tpl);
    }
    auto end() {
        auto tpl = topaz::adl_make_tuple(m_f_left.end(), m_f_right.end());
        return topaz::detail::make_zip_iterator(tpl);
    }
    auto end() const {
        auto tpl = topaz::adl_make_tuple(m_f_left.end(), m_f_right.end());
        return topaz::detail::make_zip_iterator(tpl);
    }

private:
    VolumetricField<ET, N> m_f_left;
    VolumetricField<ET, N> m_f_right;
};

template <class ET, size_t N, class Scheme> auto d_di(const Flux<ET, N>& F, Scheme scheme) {

    auto dF(F.value());
    auto delta = spatial_stepsize(F.value().grid())[Scheme::direction];
    evaluate_tiled(F.value(), dF, scheme);

    auto ret(dF);
    ret = dF / delta;
    return ret;
}

template <class ET, size_t N, class Scheme1, class Scheme2>
auto d_di(const SplitFlux<ET, N>& F, Scheme1 scheme1, Scheme2 scheme2) {

    static constexpr auto dir = Scheme1::direction;
    static_assert(dir == Scheme2::direction, "Direction mismatch in flux differentiation");

    const auto& Fl = F.left_value();
    const auto& Fr = F.right_value();

    VolumetricField<ET, N> ret(Fl.grid(), Fl.padding());

    auto Fl_int(Fl);
    auto Fr_int(Fr);

    evaluate_tiled(Fl, Fl_int, scheme1);
    evaluate_tiled(Fr, Fr_int, scheme2);

    auto dFl(Fl);
    auto dFr(Fr);
    evaluate_tiled(Fl_int, dFl, Upwind1<dir>{});
    evaluate_tiled(Fr_int, dFr, Downwind1<dir>{});

    auto delta = spatial_stepsize(Fl.grid())[dir];
    ret = (dFl + dFr) / delta;

    return ret;
}