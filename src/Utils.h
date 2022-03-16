#pragma once
#include <cmath>

/*
 * Dwie funkcje jako szablony
 */

template<typename T>
constexpr auto sinus = [](T x, T a) {
    return a*std::sin(x);
};
template<typename T>
constexpr auto cosinus = [](T x, T a) {
    return a*std::cos(x);
};


