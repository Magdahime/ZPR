/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Unit tests for randomNumberGenerator class    ///
/////////////////////////////////////////////////////////

//#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <array>
#include "Main.h"
#include "Neuron.h"
#include "RandomNumberGenerator.h"

BOOST_AUTO_TEST_SUITE(randomNumberGeneratorSuite)
BOOST_AUTO_TEST_CASE(getIntTest)
{
    double min = 0;
    double max = 9;
    std::array<int, 10> testArray{};
    int iterations = 1000000;
    double percentage = 10.0; /// percentage of occurrence of each number (based on simple probability)
    double factor = 10000.0;  /// (number:1000000)*100
    double difference = 0.1;  /// a small number to check the quality of randomness
    RandomNumberGenerator<int> rng(min, max);
    for (int i = 0; i < iterations; i++)
    {
        testArray[rng.get()]++;
    }
    for (int i = 0; i <= max; i++)
    {
        BOOST_CHECK(fabs(percentage - (testArray[i] / factor)) <= difference);
    }
}
BOOST_AUTO_TEST_CASE(getNormalTest)
{
    double min = 0;
    double max = 9;
    RandomNumberGenerator<int> rng(min, max);
    std::map<int, int> hist{};
    for (int n = 0; n < 10000; ++n)
    {
        ++hist[std::round(rng.getNormal())];
    }
    for (auto p : hist)
    {
        std::cout << p.first << ' ' << std::string(p.second / 200, '*') << '\n';
    }
}
BOOST_AUTO_TEST_SUITE_END()