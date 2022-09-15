#pragma once


struct Rk1{

    auto operator()(auto f, auto df, auto dt){
        return dt * df(f);
    }


};
