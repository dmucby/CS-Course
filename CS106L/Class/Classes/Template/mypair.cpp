//
// Created by cby on 22-7-27.
// mypair.cpp

#include "mypair.h"

template<typename First, typename Second>
First MyPair::getFirst(){
    return first;
}

template<typename Second, typename First>
Second MyPair::getSecond(){
    return second;
}