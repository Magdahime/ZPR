#include "structs/RgbHsv.h"

RGBvals convert2RGB(HSVvals &hsv)
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

HSVvals convert2HSV(RGBvals &rgb)
{
    HSVvals hsv;
    float R1 = rgb.r_ / 255;
    float B1 = rgb.b_ / 255;
    float G1 = rgb.g_ / 255;

    float Cmax = std::max({R1, G1, B1});
    float Cmin = std::min({R1, G1, B1});

    float delta = Cmax - Cmin;

    hsv.v_ = Cmax;

    if (Cmax == 0.f / 255)
        hsv.s_ = 0.0f;
    else
        hsv.s_ = (delta / Cmax);

    if (delta == 0.f / 255)
        hsv.h_ = 0.0f;
    else if (Cmax == R1)
    {
        int partial = std::round((G1 - B1) / delta);
        hsv.h_ = partial % 6;
    }
    else if (Cmax == G1)
        hsv.h_ = ((B1 - R1) / delta) + 2.0;
    else if (Cmax == B1)
        hsv.h_ = ((R1 - G1) / delta) + 4.0;

    hsv.h_ *= 60;

    if (hsv.h_ < 0)
    {
        hsv.h_ += 360;
    }

    return hsv;
}