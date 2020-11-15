#include "map.h"

unsigned char * Map::generateR(){
    float * perlinNoiseR = perlin_.generatePerlinNoise();
    float * perlinNoiseG = perlin_.generatePerlinNoise();
    float * perlinNoiseB = perlin_.generatePerlinNoise();
    for(int i = 0; i< width_*height_*4; i+=4){
        pixels_[i] = perlinNoiseR[i/4] * 255;
        pixels_[i+1] = perlinNoiseG[i/4] * 255;                                                                                                                                                                                                                                                     ;
        pixels_[i+2] = perlinNoiseB[i/4] * 255; ;
        pixels_[i+3] = 255;

    }

    return pixels_;
}
