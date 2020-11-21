#include <iostream>
#include "perlin.h"

class Map
{
    int width_;
    int height_;
    unsigned char *pixels_;
    Perlin perlin_;

public:
    Map(int width, int height) : width_(width), height_(height), perlin_(Perlin(width_, height_))
    {
        pixels_ = new unsigned char[width_ * height_ * 4];
    }
    unsigned char *generateR();
    unsigned char *generateG();
    unsigned char *generateB();

};