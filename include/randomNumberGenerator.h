/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Definition of random number generator class   ///
///     and its source code - it is supposed to       ///
///     return random integers and floats             ///
/////////////////////////////////////////////////////////
#ifndef RANDOMNUMBERGENERATOR
#define RANDOMNUMBERGENERATOR
#include "main.h"

class RandomNumberGenerator {
private:
    std::mt19937_64 generator_;
    std::uniform_real_distribution<float> distributionReal_;
    std::uniform_int_distribution<int> distributionInt_;
public:
    RandomNumberGenerator(float min, float max){
        std::random_device rd;
        this->generator_ = std::mt19937_64(rd());
        this->distributionReal_ = std::uniform_real_distribution<float>(min, max);
        this->distributionInt_ = std::uniform_int_distribution<int>(static_cast<int>(min),
                                                                 static_cast<int>(max));
    }
    double getFloat(){
        return distributionReal_(generator_);
    }
    int getInt(){
        return distributionInt_(generator_);
    }
};

#endif
