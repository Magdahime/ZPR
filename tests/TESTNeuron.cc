/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Unit tests for Neuron class                   ///
/////////////////////////////////////////////////////////

//#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "Neuron.h"
#include "RandomNumberGenerator.h"

BOOST_AUTO_TEST_SUITE(NeuronSuite)
BOOST_AUTO_TEST_CASE(MutateTest)
{
    const float mutation = 0.4;
    std::vector<float> randomWeights;
    int iterations = 1000000;
    float min = 0.0;
    float max = 100.0;
    RandomNumberGenerator<float> rng(min, max);
    for (int i = 0; i < iterations; ++i)
    {
        randomWeights.push_back(rng.get());
    }
    std::vector<float> copyRandomWeights(randomWeights);
    Neuron testNeuron(copyRandomWeights.begin(), copyRandomWeights.end());
    testNeuron.mutate();
    int numberOfMutated = 0;
    for (int i = 0; i < iterations; ++i)
    {
        if (copyRandomWeights[i] != randomWeights[i])
        {
            numberOfMutated++;
        }
    }
    BOOST_CHECK(numberOfMutated <= mutation * iterations);
}
BOOST_AUTO_TEST_SUITE_END()