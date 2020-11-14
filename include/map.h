#include <iostream>


class Map{
    int width_;
    int height_;
    unsigned char* pixels_;

    Map(int width, int height): width_(width), height_(height){
        pixels_ = new unsigned char[width_ * height_ * 4];
    }

};