#pragma once
#include <cmath>
#include <algorithm>
/**
 * \class
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
 * \class
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

RGBvals convert2RGB(HSVvals &hsv);
HSVvals convert2HSV(RGBvals &rgb);