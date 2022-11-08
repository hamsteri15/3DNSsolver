#pragma once

///
//@brief Left-biased eno f0 (left) stencil
//
//@param m2 value at i-2
//@param m1 value at i-1
//@param m0 value at i
//@return auto differentiated value at i
///
auto f0_left(auto m2, auto m1, auto m0){

    return (2.*m2 - 7.*m1 + 11.*m0)*(1./6.);
}

///
//@brief Left biased eno f1 (center) stencil
//
//@param m1 value at i-1
//@param m0 value at i
//@param p1 value at i+1
//@return auto differentiated value at i
///
auto f1_left(auto m1, auto m0, auto p1){
    
    return (-m1 + 5.*m0 + 2.*p1)*(1./6.);
}


///
//@brief Left-biased eno f1 (right) stencil
//
//@param m0 value at i
//@param p1 value at i+1
//@param p2 value at i+2
//@return auto differentiated value at i
///
auto f2_left(auto m0, auto p1, auto p2){

    return (2.*m0 + 5.*p1 - p2)*(1./6.);    
}





///
//@brief Right-biased eno f0 (left) stencil
//
//@param m2 value at i-2
//@param m1 value at i-1
//@param m0 value at i
//@return auto differentiated value at i
///
auto f0_right(auto m2, auto m1, auto m0){

    return f1_left(m2, m1, m0);
}

///
//@brief Right-biased eno f1 (center) stencil
//
//@param m1 value at i-1
//@param m0 value at i
//@param p1 value at i+1
//@return auto differentiated value at i
///
auto f1_right(auto m1, auto m0, auto p1){
    
    return f2_left(m1, m0, p1);
}

///
//@brief Right-biased eno f2 (right) stencil
//
//@param m0 value at i
//@param p1 value at i+1
//@param p2 value at i+2
//@return auto differentiated value at i
///
auto f2_right(auto m0, auto p1, auto p2){

    return f0_left(p2, p1, m0); //Note! Flipped order
}