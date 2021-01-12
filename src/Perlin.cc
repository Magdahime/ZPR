#include "Perlin.h"

/**
 * Standard constructor of Perlin Noise Generator. 
 * \param width - width of the target map
 * \param height - height of the target map
 */
Perlin::Perlin(int width, int height) : width_(width), height_(height)
{
    primaryNoise_ = new float[width * height];
}

Perlin::~Perlin(){
    delete[] primaryNoise_;
}

/**
 * Generates target array of random numbers from 0.0 to 1.0 
 */
void Perlin::generateWhiteNoise()
{
    RandomNumberGenerator<float, float> rng(0.0, 1.0);
    for (int i = 0; i < height_ * width_; ++i)
        primaryNoise_[i] = rng.get();
}

/**
 * To interpolates the values of layers of Perlin noise.
 * We decided to test two methods and choose the one with better
 * results.
 * \param value1 - first value we want to interpolate
 * \param value2 - second value we want to interpolate
 * \param alpha - relative distance betwwen the point represented by value1 and the other point - value2
 * \return - the interpolated value
 */
float Perlin::cosineInterpolation(float value1, float value2, float alpha)
{
    float angle = alpha * PI;
    float perc = (1 - std::cos(angle)) * 0.5f;
    return value1 * (1 - perc) + value2 * perc;
}


/**
 * Standard linear interpolation.
 * \param value1 - first value we want to interpolate
 * \param value2 - second value we want to interpolate
 * \param alpha - relative distance betwwen the point represented by value1 and the other point - value2
 * \return - the interpolated value
 */
float Perlin::linearInterpolation(float value1, float value2, float alpha)
{
    return value1 * (1 - alpha) + value2 * alpha;
}

/**
 * Creates one layer of Perlin noise.
 * \param octave - parameter that defines how fine the Perlin noise will be.
 * \return - array of pseudorandom numbers from 0.0 to 1.0
 */
float *Perlin::generateSmoothNoise(int octave)
{
    float *smoothNoise = new float[width_ * height_];
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

/**
 * Full Perlin Noise algorithm. It generates layers of thicker and thicker noise
 * and at the end sums up the results and normalizes the target array.
 * \return ready to use array of Perlin Noise
 */
float *Perlin::generatePerlinNoise()
{
    generateWhiteNoise();
    float persistence = 0.8f;
    float **smoothedNoises = new float *[OCTAVESNUMBER];
    float amplitude = 1.0;
    float totalAmplitude = 0.0f;
    for (int octave = 0; octave < OCTAVESNUMBER; octave++)
    {
        smoothedNoises[octave] = generateSmoothNoise(octave);
    }
    float *perlinNoise = new float[width_ * height_];
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
