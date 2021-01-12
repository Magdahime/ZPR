#include "Map.h"

unsigned char *Map::generateMapFromPerlin(Perlin &perlin)
{
    std::vector<float> perlinNoiseH = perlin.generatePerlinNoise();
    std::vector<float> perlinNoiseS = perlin.generatePerlinNoise();
    std::vector<float> perlinNoiseV = perlin.generatePerlinNoise();
    for (size_t i = 0; i < width_ * height_ * 4; i += 4)
    {
        HSVvals hsv(perlinNoiseH[i / 4] * 360, perlinNoiseS[i / 4], perlinNoiseV[i / 4]);
        HSVpixels_[i] = hsv.h_;
        HSVpixels_[i + 1] = hsv.s_;
        HSVpixels_[i + 2] = hsv.v_;
        HSVpixels_[i + 3] = 255;
        RGBvals rgb = convert2RGB(hsv);
        pixels_[i] = rgb.r_;
        pixels_[i + 1] = rgb.g_;
        pixels_[i + 2] = rgb.b_;
        pixels_[i + 3] = 255;
    }

    return pixels_;
}

