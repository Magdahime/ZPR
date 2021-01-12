#include "Perlin.h"

using namespace zpr;

Perlin::Perlin(int width, int height) : width_(width), height_(height)
{
    primaryNoise_ = std::vector<float>(width_ * height_);
}


void Perlin::generateWhiteNoise()
{
    RandomNumberGenerator<float, float> rng(0.0, 1.0);
    for (int i = 0; i < height_ * width_; ++i)
        primaryNoise_[i] = rng.get();
}


std::vector<float> Perlin::generateSmoothNoise(int octave)
{
    std::vector<float> smoothNoise (width_*height_);
    int samplePeriod = 1 << octave;
    float frequency = 1.0f / samplePeriod;

    for (int i = 0; i < width_ * height_; ++i)
    {
        int x = i % width_;
        int y = i / width_;
        int hsample0 = (x / samplePeriod) * samplePeriod;
        int hsample1 = (hsample0 + samplePeriod) % width_;
        float horizontalBlend = (x - hsample0) * frequency;
        int vsample0 = (y / samplePeriod) * samplePeriod;
        int vsample1 = (vsample0 + samplePeriod) % height_;
        float verticalBlend = (y - vsample0) * frequency;

        float top = cosineInterpolation(primaryNoise_[hsample0 + width_ * vsample0], primaryNoise_[hsample1 + width_ * vsample0], horizontalBlend);
        float bottom = cosineInterpolation(primaryNoise_[hsample0 + width_ * vsample1], primaryNoise_[hsample1 + width_ * vsample1], horizontalBlend);

        smoothNoise[i] = cosineInterpolation(top, bottom, verticalBlend);
    }
    return smoothNoise;
}

std::vector<float> Perlin::generatePerlinNoise()
{
    generateWhiteNoise();
    float persistence = 0.8f;
    std::vector<std::vector<float>> smoothedNoises (OCTAVESNUMBER, std::vector<float>(width_*height_));
    float amplitude = 1.0;
    float totalAmplitude = 0.0f;
    for (int octave = 0; octave < OCTAVESNUMBER; octave++)
    {
        smoothedNoises[octave] = generateSmoothNoise(octave);
    }
    std::vector<float> perlinNoise(width_*height_);
    for (int octave = OCTAVESNUMBER - 1; octave >= 0; octave--)
    {
        amplitude *= persistence;
        totalAmplitude += amplitude;
        for (int i = 0; i < width_ * height_; ++i)
        {
            perlinNoise[i] += smoothedNoises[octave][i] * amplitude;
        }
    }

    for (int i = 0; i < width_ * height_; ++i)
    {
        perlinNoise[i] /= totalAmplitude;
    }

    return perlinNoise;
}


float zpr::cosineInterpolation(float value1, float value2, float alpha)
{
    float angle = alpha * PI;
    float perc = (1 - std::cos(angle)) * 0.5f;
    return value1 * (1 - perc) + value2 * perc;
}
float zpr::linearInterpolation(float value1, float value2, float alpha)
{
    return value1 * (1 - alpha) + value2 * alpha;
}