#pragma once
#include <cmath>
#include <vector>
#include <sstream>

// parte da workez com foco em matematica

namespace workez{
    template<typename S>
    S soma(const std::vector<S>& lista){
        S sd = 0;
        for(S pss : lista){
            sd += pss;
        }
        return sd;
    }
}