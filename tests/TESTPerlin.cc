/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Unit tests for Perlin class                   ///
/////////////////////////////////////////////////////////

//#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "Perlin.h"

using namespace zpr;

BOOST_AUTO_TEST_SUITE(PerlinSuite)
BOOST_AUTO_TEST_CASE(LinearInterpolationTest)
{
    float epsilon = 0.0001;
    BOOST_CHECK(fabs(linearInterpolation(1,2,0.3) -1.3) < epsilon);
    BOOST_CHECK(fabs(linearInterpolation(30,45.60,0.1) - 31.56) < epsilon);
    BOOST_CHECK(fabs(linearInterpolation(10,5,0.5) - 7.5) < epsilon);
}
BOOST_AUTO_TEST_CASE(CosineInterpolationTest)
{
    float epsilon = 0.0001;
    BOOST_CHECK(fabs(cosineInterpolation(6,22,0.73) - 19.29048) < epsilon);
    BOOST_CHECK(fabs(cosineInterpolation(30.89,120.7,0.1) - 33.08780) < epsilon);
    BOOST_CHECK(fabs(cosineInterpolation(18.8,19.7,0.99) - 19.69978) < epsilon);

}
BOOST_AUTO_TEST_CASE(generatePerlinNoiseTest)
{
    Perlin perlin(1000,1000);
    std::vector<float> result = perlin.generatePerlinNoise();
    BOOST_CHECK( result.size() == 1000*1000);
}
BOOST_AUTO_TEST_SUITE_END()