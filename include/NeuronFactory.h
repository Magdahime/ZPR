/////////////////////////////////////////////////////////
///     Author: Bartłomiej Janowski                   ///
///     Singleton NeuronParameters Factory            ///
///                                                   ///
///                                                   ///
/////////////////////////////////////////////////////////
#pragma once

#pragma warning(push, 0)

#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <memory>

#pragma warning(pop)

#include "RandomNumberGenerator.h"
#include "structs/NeuronParameters.h"

const std::vector<unsigned int> LAYER_WIDTHS = {
    10,
    12,
    5};
const std::vector<unsigned int> LAYER_OFFSETS{
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
            set->push_back(std::make_shared<NeuronLayer>());
            for (NeuronParameters &neuron : (*layer))
            {
                // std::cout << " c ";
                ++i;
                //     0;
                    set->back()->emplace_back(neuron.getChild());
            }
        }
        // std::cout << " I: " << i;
        return set;
    }

    ~NeuronFactory() = default;
};
