#pragma once


struct Rk3{
    auto operator()(auto f, auto df, auto dt){
        auto k1 = df(f);
        auto k2 = df(f + 0.5*k1*dt);
        auto k3 = df(f + (3./4.)*k2*dt);
        return (dt/9.)*(2.0*k1 + 3.0*k2 + 4.0*k3);
    }

};
