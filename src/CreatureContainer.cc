/**
 * \file
 * \author Bartłomiej Janowski
 */

#include "CreatureContainer.h"

using namespace zpr;

CreatureContainer::CreatureContainer()
{
    neuronSize_ = 0;
    neuronSize_ += LAYER_WIDTHS[0];
    for (size_t i = 1; i < LAYER_WIDTHS.size(); ++i)
    {
        neuronSize_ += LAYER_WIDTHS[i];
        neuronSize_ += LAYER_WIDTHS[i - 1] * LAYER_WIDTHS[i];
    };
}

size_t CreatureContainer::getSize()
{
    return types_.size();
}

void CreatureContainer::putCreature(std::string type)
{
    putCreature(
        CreatureFactory::getInstance().createCreature(type),
        NeuronFactory::getInstance().createNeuronSet());
}
NeuronSetSPtr CreatureContainer::getNeurons(size_t index)
{
    std::vector<size_t> layerWidths;
    {
        layerWidths = LAYER_WIDTHS;
    }
    unsigned int neuronsSize = layerWidths[0];
    for (size_t i = 1; i < layerWidths.size(); ++i)
    {
        neuronsSize += layerWidths[i] + layerWidths[i - 1] * layerWidths[i];
    }
    NeuronSetSPtr neurons = std::make_shared<NeuronSet>();
    unsigned int neuronsOffset = index * neuronsSize;
    for (size_t i = 1; i < layerWidths.size(); ++i)
    {
        neuronsOffset += layerWidths[i - 1];
        neurons->emplace_back(std::make_shared<NeuronLayer>());
        for (size_t j = 0; j < layerWidths[i]; ++j)
        {
            neurons->back()->emplace_back(layerWidths[i - 1], i);
            for (size_t k = 0; k < layerWidths[i - 1]; ++k)
            {
                neurons->back()->back().weights[k] = neuronValues_[neuronsOffset++];
            }
        }
    }
    return neurons;
}
void CreatureContainer::putCreature(CreatureParametersSPtr params, NeuronSetSPtr neurons)
{
    std::lock_guard<std::mutex> lockGuard(mutex_);
    std::vector<size_t> layerWidths;
    {
        layerWidths = LAYER_WIDTHS;
    }
    unsigned int neuronsSize = layerWidths[0];
    for (size_t i = 1; i < layerWidths.size(); ++i)
    {
        neuronsSize += layerWidths[i] + layerWidths[i - 1] * layerWidths[i];
    }
    size_t index;
    /** The following checks if the creature should be added at the end of the vector,
     * or if there is free space available between existing entries
     */
    if (availableIndexes_.empty())
    {
        index = types_.size();
        types_.resize(types_.size() + 1);
        creatureValues_.resize(creatureValues_.size() + PARAMS_PER_CREATURE);
        neuronValues_.resize(neuronValues_.size() + neuronsSize);
    }
    else
    {
        if (!availableIndexes_.pop(index))
        {
            return;                             //unsuccesful pop
        }
    }

    unsigned int neuronsInitial = index * neuronsSize;
    unsigned int creaturesInitial = index * PARAMS_PER_CREATURE;
    unsigned int neuronsIndex = neuronsInitial;
    unsigned int creaturesIndex = creaturesInitial;
    types_[index] = params->type_;
    creatureValues_[creaturesIndex++] = params->energy_;
    creatureValues_[creaturesIndex++] = params->weight_;
    creatureValues_[creaturesIndex++] = params->hue_;
    creatureValues_[creaturesIndex++] = params->positionX_;
    creatureValues_[creaturesIndex++] = params->positionY_;
    creatureValues_[creaturesIndex++] = params->speedMultiplier_;
    creatureValues_[creaturesIndex++] = params->age_;
    creatureValues_[creaturesIndex++] = params->heading_;
    creatureValues_[creaturesIndex++] = params->speed_;
    creatureValues_[creaturesIndex++] = params->leftAntennaH_;
    creatureValues_[creaturesIndex++] = params->rightAntennaH_;
    creatureValues_[creaturesIndex++] = params->rearAntennaH_;
    creatureValues_[creaturesIndex++] = params->bottomAntennaH_;
    creatureValues_[creaturesIndex++] = params->popDensity_;
    neuronsIndex += layerWidths[0];
    for (unsigned int layerNo = 1; layerNo < layerWidths.size(); ++layerNo)
    {
        auto layer = (*neurons)[layerNo - 1];
        for (auto neuron : *layer)
        {
            for (float value : neuron.weights)
            {
                neuronValues_[neuronsIndex++] = value;
            }
        }
        neuronsIndex += layerWidths[layerNo];
    }
};

void CreatureContainer::delayPutCreature(CreatureParametersSPtr params, NeuronSetSPtr neurons)
{
    std::lock_guard<std::mutex> lockGuard(mutex_);
    auto data = std::make_tuple(params, neurons);
    putQueue_.push(data);
};

void CreatureContainer::putQueue()
{
    while (!putQueue_.empty())
    {
        auto val = putQueue_.front();
        auto params = std::get<0>(val);
        auto neurons = std::get<1>(val);
        putQueue_.pop(); // only pop-spot, thus "thread-safe" (at least safe enough)
        putCreature(params, neurons);
    }
}

void CreatureContainer::updateCreatureParameters(size_t index, CreatureParametersSPtr params)
{
    creatureValues_[index * PARAMS_PER_CREATURE + 0] = params->energy_;
    creatureValues_[index * PARAMS_PER_CREATURE + 1] = params->weight_;
    creatureValues_[index * PARAMS_PER_CREATURE + 2] = params->hue_;
    creatureValues_[index * PARAMS_PER_CREATURE + 3] = params->positionX_;
    creatureValues_[index * PARAMS_PER_CREATURE + 4] = params->positionY_;
    creatureValues_[index * PARAMS_PER_CREATURE + 5] = params->speedMultiplier_;
    creatureValues_[index * PARAMS_PER_CREATURE + 6] = params->age_;
    creatureValues_[index * PARAMS_PER_CREATURE + 7] = params->heading_;
    creatureValues_[index * PARAMS_PER_CREATURE + 8] = params->speed_;
    creatureValues_[index * PARAMS_PER_CREATURE + 9] = params->leftAntennaH_;
    creatureValues_[index * PARAMS_PER_CREATURE + 10] = params->rightAntennaH_;
    creatureValues_[index * PARAMS_PER_CREATURE + 11] = params->rearAntennaH_;
    creatureValues_[index * PARAMS_PER_CREATURE + 12] = params->bottomAntennaH_;
    creatureValues_[index * PARAMS_PER_CREATURE + 13] = params->popDensity_;
}
const CreatureParametersSPtr CreatureContainer::getCreatureParameters(size_t index)
{
    CreatureParametersSPtr parameters = std::make_shared<CreatureParameters>(CreatureParameters{
        types_[index],
        creatureValues_[index * PARAMS_PER_CREATURE + 0],
        creatureValues_[index * PARAMS_PER_CREATURE + 1],
        creatureValues_[index * PARAMS_PER_CREATURE + 2],
        creatureValues_[index * PARAMS_PER_CREATURE + 3],
        creatureValues_[index * PARAMS_PER_CREATURE + 4],
        creatureValues_[index * PARAMS_PER_CREATURE + 5],
        creatureValues_[index * PARAMS_PER_CREATURE + 6],
        creatureValues_[index * PARAMS_PER_CREATURE + 7],
        creatureValues_[index * PARAMS_PER_CREATURE + 8],
        creatureValues_[index * PARAMS_PER_CREATURE + 9],
        creatureValues_[index * PARAMS_PER_CREATURE + 10],
        creatureValues_[index * PARAMS_PER_CREATURE + 11],
        creatureValues_[index * PARAMS_PER_CREATURE + 12],
        creatureValues_[index * PARAMS_PER_CREATURE + 13]});
    return parameters;
};

float CreatureContainer::getCreatureX(size_t index)
{
    return creatureValues_[index * PARAMS_PER_CREATURE + 3];
};
float CreatureContainer::getCreatureY(size_t index)
{
    return creatureValues_[index * PARAMS_PER_CREATURE + 4];
};

void CreatureContainer::deleteCreature(size_t index)
{
    try
    {
        if (types_[index] == DELETED_DESIGNATOR)
        {
            throw std::exception();
        }
    }
    catch (std::exception &ex) // allows for ommiting both out-of-bound and deleted indexes
    {
        return;
    }
    types_[index] = DELETED_DESIGNATOR;
};

bool CreatureContainer::isDeleted(size_t index)
{
    return (types_[index].c_str()[0] == '_');
}

void CreatureContainer::populateNeurons(size_t index)
{
    neuronValues_[index * neuronSize_] = creatureValues_[index * PARAMS_PER_CREATURE] / energyDivisor_ - 1.f;         // energy
    neuronValues_[index * neuronSize_ + 1] = creatureValues_[index * PARAMS_PER_CREATURE + 1] / weightDivisor_ - 1.f; // weight
    neuronValues_[index * neuronSize_ + 2] = creatureValues_[index * PARAMS_PER_CREATURE + 2] / 180.f - 1.f;          // hue
    neuronValues_[index * neuronSize_ + 3] = creatureValues_[index * PARAMS_PER_CREATURE + 6] / ageDivisor_ - 1.f;    // age
    neuronValues_[index * neuronSize_ + 4] = creatureValues_[index * PARAMS_PER_CREATURE + 8] / speedDivisor_ - 1.f;  // speed
    neuronValues_[index * neuronSize_ + 5] = creatureValues_[index * PARAMS_PER_CREATURE + 9] / 180.f - 1.f;          // leftAntennaH
    neuronValues_[index * neuronSize_ + 6] = creatureValues_[index * PARAMS_PER_CREATURE + 10] / 180.f - 1.f;         // rightAntennaH
    neuronValues_[index * neuronSize_ + 7] = creatureValues_[index * PARAMS_PER_CREATURE + 11] / 180.f - 1.f;         // rearAntennaH
    neuronValues_[index * neuronSize_ + 8] = creatureValues_[index * PARAMS_PER_CREATURE + 12] / 180.f - 1.f;         // bottomAntennaH
    neuronValues_[index * neuronSize_ + 9] = creatureValues_[index * PARAMS_PER_CREATURE + 13];                       // popDensity
}

void CreatureContainer::calculateLayer(size_t index, unsigned int layer)
{
    if (layer > LAYER_WIDTHS.size() - 1 || layer < 1)
        return;
    size_t offset = neuronSize_ * index;
    for (size_t i = 0; i < LAYER_WIDTHS[layer]; ++i)
    {
        neuronValues_[offset + LAYER_OFFSETS[layer] + i] = 0.f;
        for (size_t j = 0; j < LAYER_WIDTHS[layer - 1]; ++j)
        {
            neuronValues_[offset + LAYER_OFFSETS[layer] + i] +=
                neuronValues_[offset + LAYER_OFFSETS[layer - 1] + j] *
                neuronValues_[offset + LAYER_OFFSETS[layer - 1] + (i + 1) * LAYER_WIDTHS[layer - 1] + j];
        }
        neuronValues_[offset + LAYER_OFFSETS[layer] + i] = tanh(neuronValues_[offset + LAYER_OFFSETS[layer] + i]);
    }
}

std::vector<float> CreatureContainer::getResult(size_t index)
{
    std::vector<float> result;
    unsigned int offset = neuronSize_ * index + LAYER_OFFSETS.back();
    for (unsigned int i = 0; i < LAYER_WIDTHS.back(); ++i)
    {
        result.push_back(neuronValues_[offset + i]);
    }
    return result;
}

float CreatureContainer::getCreatureValue(size_t index, unsigned int value)
{
    if (index * PARAMS_PER_CREATURE + value >= creatureValues_.size() || value > PARAMS_PER_CREATURE)
    {
        return 0.f;
    }
    if (types_[index].c_str()[0] == DELETED_DESIGNATOR.c_str()[0])
    {
        return 0.f;
    }
    return creatureValues_[index * PARAMS_PER_CREATURE + value];
};

std::vector<std::vector<float>> CreatureContainer::getNeuronStates(size_t index)
{
    std::vector<std::vector<float>> out = {};
    unsigned int neuronValuesIndex = neuronSize_ * index;
    for (size_t i = 0; i < LAYER_WIDTHS.size(); ++i)
    {
        out.emplace_back();
        for (size_t j = 0; j < LAYER_WIDTHS[i]; ++j)
        {
            out.back().push_back(neuronValues_[neuronValuesIndex++]);
        }
        neuronValuesIndex += LAYER_WIDTHS[i] * LAYER_WIDTHS[i + 1];
    }
    return out;
};