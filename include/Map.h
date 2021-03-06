#pragma once
/////////////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                           ///
///     Definition of map that will be used in simulation     ///
///     It will be generated using Perlin noise generator     ///
///     as hsv values later to be converted to rgb            ///
/////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstdlib>
#include <cmath>
#include "Perlin.h"

const int DEGREES = 360;
const int INVALID_COORDS = -1;

class Map
{
    int width_;
    int height_;
    unsigned char *pixels_;
    unsigned char *HSVpixels_;
    void scalePerlin(float *perlinH, float *perlinS, float *perlinV);
    float max3(float float1, float float2, float float3);
    float min3(float float1, float float2, float float3);

public:
    Map(int width, int height) : width_(width), height_(height)
    {
        pixels_ = new unsigned char[width_ * height_ * 4];
        HSVpixels_ = new unsigned char[width_ * height_ * 4];
    }
    Map(int width, int height, unsigned char *pixels) : width_(width), height_(height), pixels_(pixels) {}
    Map() {}
    struct RGBvals
    {
        float r_;
        float g_;
        float b_;
        RGBvals(float r, float g, float b) : r_(r), g_(g), b_(b) {}
        RGBvals() {}
    };
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
    bool compareFloat(float float1, float float2, float epsilon = 0.01f);
    inline int getWidth() { return width_; };
    inline int getHeight() { return height_; };
    inline int getPixel(int x, int y, int offset) { return pixels_[width_ * y * 4 + x * 4 + offset]; }
    inline int getPixelH(int x, int y)
    {
        if (x < width_ && y < height_ && x >= 0 && y>= 0)
            return HSVpixels_[width_ * y * 4 + x * 4];
        return INVALID_COORDS;
    }
};