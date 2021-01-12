#pragma once

#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <iostream>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

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
    Perlin() = delete;
    Perlin(const Perlin &) = delete;
    Perlin(Perlin &&) = delete;
    Perlin &operator=(Perlin &&) = delete;
    Perlin &operator=(const Perlin &) = delete;
public:
    Perlin(int width, int height);
    ~Perlin();
    void generateWhiteNoise();
    float *generateSmoothNoise(int octave);
    float cosineInterpolation(float value1, float value2, float alpha);
    float linearInterpolation(float value1, float value2, float alpha);
    float *generatePerlinNoise();
};