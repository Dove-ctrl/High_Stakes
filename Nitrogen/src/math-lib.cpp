#include "math-lib.h"

float Sin(float deg){
    return sin((deg * pi) / 180.0);
}

float Cos(float deg){
    return cos((deg * pi) / 180.0);
}

float arctan(float tan){
    return atan(tan) * 180.0 / pi;
}

float arcsin(float sin){
    return asin(sin) * 180.0 / pi;
}

int sign(float input){
    return (input >= 0 ? (input == 0 ? 0 : 1) : -1);
}