#pragma once
#include <cmath>
#include <algorithm>
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
/**
 * Converter from hsv values to rgb
 * It is an implementation of algorithm found on the internet
 * https://www.rapidtables.com/convert/color/hsv-to-rgb.html 
 * \return RGB values of the provided HSV
 */
RGBvals convert2RGB(HSVvals &hsv);
/**
 * Converter from rgb values to  hsv
 * It is an implementation of algorithm found on the internet
 * https://www.rapidtables.com/convert/color/rgb-to-hsv.html
 * \return HSV values of the provided RGB
 */
HSVvals convert2HSV(RGBvals &rgb);