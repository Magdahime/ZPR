#pragma once

#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <vector>
#include <memory>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

#include "RandomNumberGenerator.h"

constexpr float STDDEV_NEURON = 0.5f;

/**
 * \author Bartłomiej Janowski
 * 
 * This structs represents the entirety of single neuron's parameters (weights). 
 * Based on this neuron, the neuron values storage of \link CreatureContainer is populated.
 */
struct NeuronParameters
{
    unsigned int inputsNo;
    unsigned int layerNo;
    std::vector<float> weights;

    inline thread_local static RandomNumberGenerator<float> rng = RandomNumberGenerator<float>(-1.f, 1.f, 0.f, STDDEV_NEURON);

    NeuronParameters(
        unsigned int inputsNo,
        unsigned int layerNo, bool lastLayer = false) : inputsNo(inputsNo), layerNo(layerNo)
    {
        float lowerBound, upperBound;
        if(!lastLayer){
            lowerBound = (-1) / sqrt(inputsNo);
            upperBound = 1 / sqrt(inputsNo);
        }
        else {
            lowerBound = -1;
            upperBound = 1;
        }
        for (unsigned int i = 0; i < inputsNo; ++i)
        {
            weights.push_back(rng.get(lowerBound, upperBound));
        }
    }

    NeuronParameters() = delete;

    NeuronParameters getChild()
    {
        NeuronParameters child(inputsNo, layerNo);
        for (unsigned int i = 0; i < inputsNo; ++i)
        {
            child.weights[i] = weights[i] + rng.getNormal(0, 0.01 * sqrt(inputsNo));
        }
        return child;
    }
};

using NeuronParametersSPtr = std::shared_ptr<NeuronParameters>;
using NeuronLayer = std::vector<NeuronParameters>;
using NeuronLayerSPtr = std::shared_ptr<NeuronLayer>;
using NeuronSet = std::vector<NeuronLayerSPtr>;
using NeuronSetSPtr = std::shared_ptr<NeuronSet>;