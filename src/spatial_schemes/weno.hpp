#pragma once

#include <array>

#include <jada.hpp>
#include "spatial_schemes/eno.hpp"

template <typename T>
std::array<T, 3> weights_shu(T f_m2, T f_m1, T f_, T f_p1, T f_p2, std::array<double, 3> d) {

    const double oneperfour        = 1. / 4.;
    const double thirteenpertwelve = 13. / 12.;
    const double epsilon_shu       = 1E-6;

    T temp1;
    T temp2;
    T b0;
    T b1;
    T b2;

    temp1 = f_m2 - 2. * f_m1 + f_;
    temp2 = f_m2 - 4. * f_m1 + 3. * f_;
    b0    = thirteenpertwelve * temp1 * temp1 + oneperfour * temp2 * temp2;

    temp1 = f_m1 - 2. * f_ + f_p1;
    temp2 = f_m1 - f_p1;
    b1    = thirteenpertwelve * temp1 * temp1 + oneperfour * temp2 * temp2;

    temp1 = f_ - 2. * f_p1 + f_p2;
    temp2 = 3. * f_ - 4. * f_p1 + f_p2;
    b2    = thirteenpertwelve * temp1 * temp1 + oneperfour * temp2 * temp2;

    T alpha_0 = d[0] / ((epsilon_shu + b0) * (epsilon_shu + b0));
    T alpha_1 = d[1] / ((epsilon_shu + b1) * (epsilon_shu + b1));
    T alpha_2 = d[2] / ((epsilon_shu + b2) * (epsilon_shu + b2));

    T alpha = alpha_0 + alpha_1 + alpha_2;

    return {alpha_0 / alpha, alpha_1 / alpha, alpha_2 / alpha};
}


auto weno_left(auto m2, auto m1, auto m0, auto p1, auto p2){

    auto f0 = f0_left(m2, m1, m0);
    auto f1 = f1_left(m1, m0, p1);
    auto f2 = f2_left(m0, p1, p2);

    constexpr std::array<double, 3> d = {0.1, 0.6, 0.3};

    auto w = weights_shu(m2, m1, m0, p1, p2, d);

    return w[0] * f0 + w[1] * f1 + w[2] * f2;
}

auto weno_right(auto m2, auto m1, auto m0, auto p1, auto p2){

    auto f0 = f0_right(m2, m1, m0);
    auto f1 = f1_right(m1, m0, p1);
    auto f2 = f2_right(m0, p1, p2);

    constexpr std::array<double, 3> d = {0.3, 0.6, 0.1};

    auto w = weights_shu(m2, m1, m0, p1, p2, d);

    return w[0] * f0 + w[1] * f1 + w[2] * f2;
}

struct Weno_left{

    static constexpr size_t padding = 2;

    auto operator()(auto f) const {

        auto m2 = f(-2);
        auto m1 = f(-1);
        auto m0 = f(0);
        auto p1 = f(1);
        auto p2 = f(2);

        return weno_left(m2, m1, m0, p1, p2);

    }

};

struct Weno_right{

    static constexpr size_t padding = 2;

    auto operator()(auto f) const {

        auto m2 = f(-2);
        auto m1 = f(-1);
        auto m0 = f(0);
        auto p1 = f(1);
        auto p2 = f(2);

        return weno_right(m2, m1, m0, p1, p2);
    }

};