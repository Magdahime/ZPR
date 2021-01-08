/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Source code of map class                      ///
/////////////////////////////////////////////////////////

#include "Map.h"

unsigned char *Map::generateMapFromPerlin(Perlin &perlin)
{
    float *perlinNoiseH = perlin.generatePerlinNoise();
    float *perlinNoiseS = perlin.generatePerlinNoise();
    float *perlinNoiseV = perlin.generatePerlinNoise();
    for (int i = 0; i < width_ * height_ * 4; i += 4)
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

//////////////////////////////////////////////////////////////////////////////
///     Converter from hsv values to rgb                                   ///
///     It is an implementation of algorithm                               ///
///     found on the internet                                              ///
///     https://www.rapidtables.com/convert/color/hsv-to-rgb.html          ///
//////////////////////////////////////////////////////////////////////////////

Map::RGBvals Map::convert2RGB(Map::HSVvals &hsv)
{
    float C = hsv.v_ * hsv.s_;
    float partial = (hsv.h_ / 60.0);
    float X = C * (1 - fabs(fmod(partial, 2.f) - 1));
    float m = hsv.v_ - C;
    float R1, G1, B1;

    if (hsv.h_ >= 0.0 && hsv.h_ < 60.0f)
    {
        R1 = C;
        G1 = X;
        B1 = 0.0f;
    }
    else if (hsv.h_ >= 60.0 && hsv.h_ < 120.0f)
    {
        R1 = X;
        G1 = C;
        B1 = 0.0f;
    }
    else if (hsv.h_ >= 120.0 && hsv.h_ < 180.0f)
    {
        R1 = 0.0f;
        G1 = C;
        B1 = X;
    }
    else if (hsv.h_ >= 180.0 && hsv.h_ < 240.0f)
    {
        R1 = 0.0f;
        G1 = X;
        B1 = C;
    }
    else if (hsv.h_ >= 240.0 && hsv.h_ < 300.0f)
    {
        R1 = X;
        G1 = 0.0f;
        B1 = C;
    }
    else
    {
        R1 = C;
        G1 = 0.0f;
        B1 = X;
    }
    return RGBvals(std::round((R1 + m) * 255), std::round((G1 + m) * 255), std::round((B1 + m) * 255));
}

//////////////////////////////////////////////////////////////////////
///     Converter from rgb values to hsv                           ///
///     It is an implementation of algorithm                       ///
///     found on the internet                                      ///
///     https://www.rapidtables.com/convert/color/rgb-to-hsv.html  ///
//////////////////////////////////////////////////////////////////////

Map::HSVvals Map::convert2HSV(Map::RGBvals &rgb)
{
    HSVvals hsv;
    float R1 = rgb.r_ / 255;
    float B1 = rgb.b_ / 255;
    float G1 = rgb.g_ / 255;

    float Cmax = max3(R1, G1, B1);
    float Cmin = min3(R1, G1, B1);

    float delta = Cmax - Cmin;

    hsv.v_ = Cmax;

    if (compareFloat(Cmax, 0.0f))
        hsv.s_ = 0.0f;
    else
        hsv.s_ = (delta / Cmax);

    if (compareFloat(delta, 0.0f))
        hsv.h_ = 0.0f;
    else if (compareFloat(Cmax, R1))
    {
        int partial = std::round((G1 - B1) / delta);
        hsv.h_ = partial % 6;
    }
    else if (compareFloat(Cmax, G1))
        hsv.h_ = ((B1 - R1) / delta) + 2.0;
    else if (compareFloat(Cmax, B1))
        hsv.h_ = ((R1 - G1) / delta) + 4.0;

    hsv.h_ *= 60;

    if (hsv.h_ < 0)
    {
        hsv.h_ += 360;
    }

    return hsv;
}

float Map::max3(float float1, float float2, float float3)
{

    if (float1 >= float2 && float1 >= float3)
        return float1;
    else if (float2 >= float1 && float2 >= float3)
        return float2;
    else if (float3 >= float1 && float3 >= float2)
        return float3;
    return 0.0f;
}

float Map::min3(float float1, float float2, float float3)
{
    if (float1 <= float2 && float1 <= float3)
        return float1;
    else if (float2 <= float1 && float2 <= float3)
        return float2;
    else if (float3 <= float1 && float3 <= float2)
        return float3;
    return 0.0f;
}

bool Map::compareFloat(float float1, float float2, float epsilon)
{
    if (fabs(float1 - float2) < epsilon)
        return true;
    return false;
}