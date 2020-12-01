/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Definition of perlin noise generator          ///
/////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include "RandomNumberGenerator.h"

const int OCTAVESNUMBER =7;
const float PI = 3.14159;

class Perlin
{
    const int width_;
    const int height_;
    float * primaryNoise_;

public:
    Perlin(int width, int height);
    void generateWhiteNoise();
    float * generateSmoothNoise(int octave);
    float cosineInterpolation(float value1, float value2, float alpha);
    float linearInterpolation(float value1, float value2, float alpha);
    float * generatePerlinNoise();
};