#pragma once

#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <iostream>
#include <array>

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
    std::vector<float> primaryNoise_;
    Perlin() = delete;
    Perlin(const Perlin &) = delete;
    Perlin(Perlin &&) = delete;
    Perlin &operator=(Perlin &&) = delete;
    Perlin &operator=(const Perlin &) = delete;
public:

    /**
     * Standard constructor of Perlin Noise Generator. 
     * \param width width of the target map
     * \param height  height of the target map
     */
    Perlin(int width, int height);
    ~Perlin() = default;
    /**
     * Generates target array of random numbers from 0.0 to 1.0 
     */
    void generateWhiteNoise();
    /**
     * Creates one layer of Perlin noise.
     * \param octave parameter that defines how fine the Perlin noise will be.
     * \return array of pseudorandom numbers from 0.0 to 1.0
     */
    std::vector<float> generateSmoothNoise(int octave);
    /**
     * Full Perlin Noise algorithm. It generates layers of thicker and thicker noise
     * and at the end sums up the results and normalizes the target array.
     * \return ready to use array of Perlin Noise
     */
    std::vector<float> generatePerlinNoise();
};
    /**
     * To interpolates the values of layers of Perlin noise.
     * We decided to test two methods and choose the one with better
     * results.
     * \param value1 first value we want to interpolate
     * \param value2 second value we want to interpolate
     * \param alpha relative distance betwwen the point represented by value1 and the other point - value2
     * \return the interpolated value
     */
    float cosineInterpolation(float value1, float value2, float alpha);
    
    
    /**
     * Standard linear interpolation.
     * \param value1 first value we want to interpolate
     * \param value2 second value we want to interpolate
     * \param alpha relative distance betwwen the point represented by value1 and the other point - value2
     * \return the interpolated value
     */
    float linearInterpolation(float value1, float value2, float alpha);