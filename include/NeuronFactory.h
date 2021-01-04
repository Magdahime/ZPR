/////////////////////////////////////////////////////////
///     Author: Bartłomiej Janowski                   ///
///     Singleton NeuronParameters Factory            ///
///                                                   ///
///                                                   ///
/////////////////////////////////////////////////////////

#pragma once
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>

#include "RandomNumberGenerator.h"

const float STDDEV_NEURON = 0.5f;

const std::vector<unsigned int> LAYER_WIDTHS = {
    10,
    12,
    5};
const std::vector<unsigned int> LAYER_OFFSETS{
    0,
    130,
    202};

struct NeuronParameters;

using NeuronParametersSPtr = std::shared_ptr<NeuronParameters>;
using NeuronLayer = std::vector<NeuronParameters>;
using NeuronLayerSPtr = std::shared_ptr<NeuronLayer>;
using NeuronSet = std::vector<NeuronLayerSPtr>;
using NeuronSetSPtr = std::shared_ptr<NeuronSet>;

struct NeuronParameters
{
    unsigned int inputsNo;
    unsigned int layerNo;
    std::vector<float> weights;

    inline static RandomNumberGenerator<float> rng = RandomNumberGenerator<float>(-1.f, 1.f, 0.f, STDDEV_NEURON);

    NeuronParameters(
        unsigned int inputsNo,
        unsigned int layerNo) : inputsNo(inputsNo), layerNo(layerNo)
    {
        for (int i = 0; i < inputsNo; i++)
        {
            weights.push_back(rng.get());
        }
        // std::cout<<"Ino Lno I:\t"<<inputsNo<<"\t"<<layerNo<<"\t"<<i<<"\n";
    }

    NeuronParameters() = default;

    NeuronParameters getChild()
    {
        NeuronParameters child(inputsNo, layerNo);
        for (int i = 0; i < inputsNo; i++)
        {
            child.weights[i] = weights[i] + rng.getNormal(0, 0.02); //alert MAGIC
        }
        return child;
    }
};

class NeuronFactory
{
    NeuronFactory() = default;
    NeuronFactory(const NeuronFactory &) = delete;
    NeuronFactory &operator=(const NeuronFactory &) = delete;

    NeuronSetSPtr createNeuronSet(const std::vector<unsigned int> &layersWidths)
    {
        NeuronSetSPtr set = std::make_shared<NeuronSet>();
        unsigned int lastLayerWidth, currLayerWidth;
        for (int layer = 1; layer < layersWidths.size(); ++layer)
        {
            set->push_back(std::make_shared<NeuronLayer>());
            lastLayerWidth = layersWidths[layer - 1];
            currLayerWidth = layersWidths[layer];
            for (int neuron = 0; neuron < currLayerWidth; ++neuron)
            {
                set->back()->emplace_back(NeuronParameters(lastLayerWidth, layer));
            }
        }
        return set;
    };

public:
    static NeuronFactory &getInstance()
    {
        static NeuronFactory instance;
        return instance;
    }

    NeuronSetSPtr createNeuronSet()
    {
        return createNeuronSet(LAYER_WIDTHS);
    }

    NeuronSetSPtr createChild(NeuronSetSPtr parent)
    {
        NeuronSetSPtr set = std::make_shared<NeuronSet>();
        int i = 0;
        // std::cout << " a "<<parent->size()<<" ";
        for (NeuronLayerSPtr layer : (*parent))
        {
            // std::cout << " b ";
            // --i;
            // set->push_back(std::make_shared<NeuronLayer>());
            for (NeuronParameters &neuron : (*layer))
            {
                // std::cout << " c ";
                ++i;
                //     0;
                //     // set->back()->emplace_back(neuron.getChild());
            }
        }
        // std::cout << " I: " << i;
        return set;
    }

    ~NeuronFactory() = default;
};
