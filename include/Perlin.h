#pragma once

#pragma warning(push, 0)

#include <iostream>

#pragma warning(pop)

#include "RandomNumberGenerator.h"

const int OCTAVESNUMBER = 7;
const float PI = 3.14159;

/**
 * \author Magdalena Majkowska
 * This is the Perlin's Noise generator, it was supposed to create more realistic map for the creatures.
 * It generates a set of arrays  of Perlin's noise and at the end interpolates them 
 * to achieve more realistic structures on the map.
 */

class Perlin
{
    const int width_;
    const int height_;
    float *primaryNoise_;

public:
    Perlin(int width, int height);
    void generateWhiteNoise();
    float *generateSmoothNoise(int octave);
    float cosineInterpolation(float value1, float value2, float alpha);
    float linearInterpolation(float value1, float value2, float alpha);
    float *generatePerlinNoise();
};