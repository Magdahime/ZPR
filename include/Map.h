#pragma once

#pragma warning(push, 0)

#include <iostream>
#include <cstdlib>
#include <cmath>

#pragma warning(pop)

#include "Perlin.h"

constexpr int DEGREES = 360;
constexpr int INVALID_COORDS = -1;
/**
 * \author Magdalena Majkowska
 * 
 * Map class, holding the pixel data of the simulation as a simple 1D array.
 */
class Map
{
    int width_;
    int height_;
    unsigned char *pixels_;
    unsigned char *HSVpixels_;
    void scalePerlin(float *perlinH, float *perlinS, float *perlinV);
    float max3(float float1, float float2, float float3);
    float min3(float float1, float float2, float float3);

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

    /**
    * RGBvals are used to store the RGB values of one pixel on the map.
    * They are used during converting colours from HSV to RGB and the other way round.
    */
    struct RGBvals
    {
        float r_;
        float g_;
        float b_;
        RGBvals(float r, float g, float b) : r_(r), g_(g), b_(b) {}
        RGBvals() {}
    };
    /**
    * HSVvals are used to store the HSV values of one pixel on the map.
    * They are used during converting colours from HSV to RGB and the other way round.
    */
    struct HSVvals
    {
        float h_;
        float s_;
        float v_;
        HSVvals(float h, float s, float v) : h_(h), s_(s), v_(v) {}
        HSVvals() {}
    };
    unsigned char *generateMapFromPerlin(Perlin &perlin);
    HSVvals convert2HSV(RGBvals &rgb);
    RGBvals convert2RGB(HSVvals &hsv);
    inline int getWidth() { return width_; };
    inline int getHeight() { return height_; };
    /**
    * Gets value of the selected pixel from the RGB version of the map
    * \param x - x coordinate on the map
    * \param y - y coordinate on the map
    * \param offset - used to acces the certain value of colour 0 - red, 1 - green, 2 - blue
    * \return value of the pixel
    */
    inline int getPixel(int x, int y, unsigned int offset)
    {
        if (x < width_ && y < height_ && x >= 0 && y >= 0 && offset < 4)
            return pixels_[width_ * y * 4 + x * 4 + offset];
        return INVALID_COORDS;
    }
    /**
    * Gets value of the selected pixel from the HSV version of the map
    * \param x - x coordinate on the map
    * \param y - y coordinate on the map
    * \param offset - used to acces the certain value of colour 0 - red, 1 - green, 2 - blue
    * \return value of the pixel
    */
    inline int getPixelH(int x, int y)
    {
        if (x < width_ && y < height_ && x >= 0 && y >= 0)
            return HSVpixels_[width_ * y * 4 + x * 4];
        return INVALID_COORDS;
    }
};