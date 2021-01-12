#pragma once

#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

#include "RandomNumberGenerator.h"
#include "structs/NeuronParameters.h"

/**
 * Default layer widths for the NN
 */
const std::vector<size_t> LAYER_WIDTHS = {
    10,
    12,
    5};
/**
 * Default layer offsets (the index of a single set at which the values for a layer start) for the NN
 */
const std::vector<size_t> LAYER_OFFSETS{
    0,
    130,
    202};

/**
 * \author Bartłomiej Janowski
 * 
 * This is the singleton Neuron Factory, which allows for creation of respective NeuronParameters
 * at random or based on parents' neurons. NeuronParameters are later inputted into
 * \link CreatureContainer and used in the Simulation
 */
class NeuronFactory
{
    NeuronFactory() = default;
    NeuronFactory(const NeuronFactory &) = delete;
    NeuronFactory(NeuronFactory &&) = delete;
    NeuronFactory &operator=(NeuronFactory &&) = delete;
    NeuronFactory &operator=(const NeuronFactory &) = delete;

    /**
     * This function creates a NeuronSet for layer widths as specified,with all but last layers 
     * having weights normalized between -1/sqrt(no. of neurons on previous layer) and 
     * 1/sqrt(no. of neurons on previous layer).
     */
    NeuronSetSPtr createNeuronSet(const std::vector<size_t> &layersWidths)
    {
        NeuronSetSPtr set = std::make_shared<NeuronSet>();
        size_t lastLayerWidth, currLayerWidth;
        for (size_t layer = 1; layer < layersWidths.size(); ++layer)
        {
            set->push_back(std::make_shared<NeuronLayer>());
            lastLayerWidth = layersWidths[layer - 1];
            currLayerWidth = layersWidths[layer];
            for (size_t neuron = 0; neuron < currLayerWidth; ++neuron)
            {
                set->back()->emplace_back(NeuronParameters(lastLayerWidth, layer, layer == layersWidths.size() - 1));
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
    /**
     * Function returning default size NeuronSet
     */
    NeuronSetSPtr createNeuronSet()
    {
        return createNeuronSet(LAYER_WIDTHS);
    }

    /**
     * Function creating neurons based od parent's ones, mutating values slightly.
     */
    NeuronSetSPtr createChild(NeuronSetSPtr parent)
    {
        NeuronSetSPtr set = std::make_shared<NeuronSet>();
        int i = 0;
        for (NeuronLayerSPtr layer : (*parent))
        {
            set->push_back(std::make_shared<NeuronLayer>());
            for (NeuronParameters &neuron : (*layer))
            {
                ++i;
                set->back()->emplace_back(neuron.getChild());
            }
        }
        return set;
    }

    ~NeuronFactory() = default;
};
