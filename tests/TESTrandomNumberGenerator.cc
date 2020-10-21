/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Unit tests for randomNumberGenerator class    ///
/////////////////////////////////////////////////////////
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE RANDOM_NUMBER_GENERATOR
#include <boost/test/included/unit_test.hpp>
#include <main.h>
#include "randomNumberGenerator.h"

BOOST_AUTO_TEST_SUITE( randomNumberGeneratorSuite )
BOOST_AUTO_TEST_CASE( getIntTest){
    double min = 0;
    double max = 9;
    std::array<int,10> testArray{};
    int iterations = 1000000;
    double percentage = 10.0; /// percentage of occurrence of each number (based on simple probability)
    double factor = 10000.0; /// (number:1000000)*100
    double difference = 0.1; /// a small number to check the quality of randomness
    RandomNumberGenerator<int> rng(min, max);
    for(int i=0; i< iterations; i++){
        testArray[rng.get()]++;
    }
    for(int i=0; i<= max; i++){
        BOOST_CHECK(fabs(percentage - (testArray[i]/factor)) <= difference );
    }
}
BOOST_AUTO_TEST_SUITE_END()