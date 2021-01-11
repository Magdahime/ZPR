#pragma once

#pragma warning(push, 0)

#include <iostream>
#include <cmath>

#pragma warning(pop)

#include "Perlin.h"
#include "structs/RgbHsv.h"

constexpr int DEGREES = 360;
constexpr int INVALID_COORDS = -1;
/**
 * \author Magdalena Majkowska
 * 
 * Map class, holding the pixel data of the simulation as a simple 1D array.
 */
class Map
{
    unsigned int width_;
    unsigned int height_;
    unsigned char *pixels_;
    unsigned char *HSVpixels_;
    void scalePerlin(float *perlinH, float *perlinS, float *perlinV);

    Map(const Map &) = delete;
    Map(Map &&) = delete;
    Map &operator=(Map &&) = delete;
    Map &operator=(const Map &) = delete;
    Map() = delete;

public:
    Map(int width, int height) : width_(width), height_(height)
    {
        pixels_ = new unsigned char[width_ * height_ * 4];
        HSVpixels_ = new unsigned char[width_ * height_ * 4];
    };
    ~Map()
    {
        delete[] pixels_;
        delete[] HSVpixels_;
    };
    Map(int width, int height, unsigned char *pixels) : width_(width), height_(height), pixels_(pixels){};
    unsigned char *generateMapFromPerlin(Perlin &perlin);
    inline int getWidth() { return width_; };
    inline int getHeight() { return height_; };
    /**
    * Gets value of the selected pixel from the RGB version of the map
    * \param x - x coordinate on the map
    * \param y - y coordinate on the map
    * \param offset - used to acces the certain value of colour 0 - red, 1 - green, 2 - blue
    * \return value of the pixel
    */
    inline short getPixel(unsigned int x, unsigned int y, unsigned int offset)
    {
        if (x < width_ && y < height_ && x >= 0 && y >= 0 && offset < 4)
            return pixels_[width_ * y * 4 + x * 4 + offset];
        return INVALID_COORDS;
    };
    /**
    * Gets value of the selected pixel from the HSV version of the map
    * \param x - x coordinate on the map
    * \param y - y coordinate on the map
    * \param offset - used to acces the certain value of colour 0 - red, 1 - green, 2 - blue
    * \return value of the pixel
    */
    inline short getPixelH(unsigned int x, unsigned int y, unsigned int offset = 0)
    {
        if (x < width_ && y < height_ && x >= 0 && y >= 0 && offset < 4)
            return HSVpixels_[width_ * y * 4 + x * 4 + offset];
        return INVALID_COORDS;
    }
};

