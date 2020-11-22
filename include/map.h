#include <iostream>
#include <cstdlib>
#include <cmath>
#include "perlin.h"

class Map
{
    int width_;
    int height_;
    unsigned char *pixels_;
    float max3(float float1, float float2, float float3);
    float min3(float float1, float float2, float float3);

public:
    Map(int width, int height) : width_(width), height_(height)
    {
        pixels_ = new unsigned char[width_ * height_ * 4];
    }
    Map(int width, int height, unsigned char* pixels): width_(width), height_(height), pixels_(pixels){}
    Map(){}
    struct RGB{
    float r_;
    float g_;
    float b_;
    RGB(float r, float g, float b) : r_(r), g_(g), b_(b){}
    RGB(){}
    };
    struct HSV{
    float h_;
    float s_;
    float v_;
    HSV(float h, float s, float v) : h_(h), s_(s), v_(v){}
    HSV(){}
    };
    unsigned char *generateMapFromPerlin(Perlin& perlin);
    HSV convert2HSV(RGB& rgb);
    RGB convert2RGB(HSV& hsv);
    bool compareFloat(float float1, float float2, float epsilon = 0.01f);

};