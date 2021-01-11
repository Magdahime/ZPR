/////////////////////////////////////////////////////////
///     Author: Bartłomiej Janowski                   ///
/////////////////////////////////////////////////////////

#include "CreatureContainer.h"

CreatureContainer::CreatureContainer()
{
    neuronSize_ = 0;
    neuronSize_ += LAYER_WIDTHS[0];
    for (int i = 1; i < LAYER_WIDTHS.size(); ++i)
    {
        neuronSize_ += LAYER_WIDTHS[i];
        neuronSize_ += LAYER_WIDTHS[i - 1] * LAYER_WIDTHS[i];
    };
}

unsigned int CreatureContainer::getSize()
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
    std::vector<unsigned int> layerWidths;
    {
        layerWidths = LAYER_WIDTHS;
    }
    unsigned int neuronsSize = layerWidths[0];
    for (int i = 1; i < layerWidths.size(); i++)
    {
        neuronsSize += layerWidths[i] + layerWidths[i - 1] * layerWidths[i];
    }
    NeuronSetSPtr neurons = std::make_shared<NeuronSet>();
    unsigned int neuronsOffset = index * neuronsSize;
    for (int i = 1; i < layerWidths.size(); i++)
    {
        neuronsOffset += layerWidths[i - 1];
        neurons->emplace_back(std::make_shared<NeuronLayer>());
        for (int j = 0; j < layerWidths[i]; j++)
        {
            neurons->back()->emplace_back(layerWidths[i - 1], i);
            for (int k = 0; k < layerWidths[i - 1]; k++)
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
    std::vector<unsigned int> layerWidths;
    {
        layerWidths = LAYER_WIDTHS;
    }
    unsigned int neuronsSize = layerWidths[0];
    for (int i = 1; i < layerWidths.size(); i++)
    {
        neuronsSize += layerWidths[i] + layerWidths[i - 1] * layerWidths[i];
    }
    size_t index;
    if (availableIndexes_.empty())
    {
        index = types_.size();
        types_.resize(types_.size() + 1);
        creatureValues_.resize(creatureValues_.size() + PARAMS_PER_CREATURE);
        neuronValues_.resize(neuronValues_.size() + neuronsSize);
        // std::cout << "\nSurvived resizing... NSize: "<<neuronValues_.size()<<"\n"; //alert DEBUG COUT
    }
    else
    {
        if (!availableIndexes_.pop(index))
        {
            std::cout << "\nunsuccesful pop\n"; // alert DEBUG
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
    // std::cout << "\nSurvived creatureValues_... NI: "<<neuronsIndex<<"\n"; //alert DEBUG COUT
    //std::cout<<"INIT SIZE\t"<<neuronValues_.size()<<"\t"<<layerWidths.size()<<"\n";
    for (unsigned int layerNo = 1; layerNo < layerWidths.size(); ++layerNo)
    {
        // std::cout<<"I'm still alive... neurons->size "<<(*neurons).size()<<"\n"; //alert DEBUG COUT
        auto layer = (*neurons)[layerNo - 1];
        for (auto neuron : *layer)
        {
            // std::cout<<"I'm still still alive...\n"; //alert DEBUG COUT
            for (float value : neuron.weights)
            {
                neuronValues_[neuronsIndex++] = value;
            }
        }
        // std::cout<<"NIndex: "<<neuronsIndex<<"\n"; //alert DEBUG COUT
        neuronsIndex += layerWidths[layerNo];
        //std::cout<<"LATER SIZE\t"<<neuronValues_.size()<<"\n";
    }
    // std::cout << "\nSurvived neuronValues_...\n"; //alert DEBUG COUT
    // creatures_.emplace_back(
    //     Creature(
    //         weak_from_this(),
    //         layerWidths,
    //         neuronsInitial,
    //         neuronValues_.size() - 1,
    //         creaturesInitial,
    //         creatureValues_.size() - 1,
    //         types_.size() - 1));
};

void CreatureContainer::delayPutCreature(CreatureParametersSPtr params, NeuronSetSPtr neurons)
{
    std::lock_guard<std::mutex> lockGuard(mutex_);
    auto data = std::make_tuple(params, neurons);
    putQueue_.push(data);
};

void CreatureContainer::putQueue()
{
    // std::lock_guard<std::mutex> lockGuard(mutex_);
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

const float CreatureContainer::getCreatureX(size_t index)
{
    return creatureValues_[index * PARAMS_PER_CREATURE + 3];
};
const float CreatureContainer::getCreatureY(size_t index)
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
    catch (std::exception ex)
    {
        return;
    }
    types_[index] = DELETED_DESIGNATOR;
    if (availableIndexes_.push(index))
        std::cout << "\npush failed\n"; // alert DEBUG
};

bool CreatureContainer::isDeleted(size_t index)
{
    return (types_[index].c_str()[0] == '_');
}

void CreatureContainer::populateNeurons(size_t index)
{
    neuronValues_[index * neuronSize_] = creatureValues_[index * PARAMS_PER_CREATURE] / 100.f;          // energy //alert MAGIC
    neuronValues_[index * neuronSize_ + 1] = creatureValues_[index * PARAMS_PER_CREATURE + 1] / 100.f;  // weight //alert MAGIC
    neuronValues_[index * neuronSize_ + 2] = creatureValues_[index * PARAMS_PER_CREATURE + 2] / 360.f;  // hue
    neuronValues_[index * neuronSize_ + 3] = creatureValues_[index * PARAMS_PER_CREATURE + 6] / 100.f;  // age //alert MAGIC
    neuronValues_[index * neuronSize_ + 4] = creatureValues_[index * PARAMS_PER_CREATURE + 8];          // speed
    neuronValues_[index * neuronSize_ + 5] = creatureValues_[index * PARAMS_PER_CREATURE + 9] / 360.f;  // leftAntennaH
    neuronValues_[index * neuronSize_ + 6] = creatureValues_[index * PARAMS_PER_CREATURE + 10] / 360.f; // rightAntennaH
    neuronValues_[index * neuronSize_ + 7] = creatureValues_[index * PARAMS_PER_CREATURE + 11] / 360.f; // rearAntennaH
    neuronValues_[index * neuronSize_ + 8] = creatureValues_[index * PARAMS_PER_CREATURE + 12] / 360.f; // bottomAntennaH
    neuronValues_[index * neuronSize_ + 9] = creatureValues_[index * PARAMS_PER_CREATURE + 13];         // popDensity
}

void CreatureContainer::calculateLayer(size_t index, unsigned int layer)
{
    if (layer > LAYER_WIDTHS.size() - 1 || layer < 1)
        return;
    unsigned int inputs = LAYER_WIDTHS[layer - 1];
    unsigned int outputs = LAYER_WIDTHS[layer];
    unsigned int offset = neuronSize_ * index;
    //std::cout<<"I L O S:\t"<<index<<"\t"<<layer<<"\t"<<offset<<"\t"<<neuronValues_.size()<<"\n";
    for (int i = 0; i < LAYER_WIDTHS[layer]; i++)
    {
        neuronValues_[offset + LAYER_OFFSETS[layer] + i] = 0.f;
        for (int j = 0; j < LAYER_WIDTHS[layer - 1]; j++)
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
    for (int i = 0; i < LAYER_WIDTHS.size(); i++)
    {
        out.emplace_back();
        for (int j = 0; j < LAYER_WIDTHS[i]; j++)
        {
            out.back().push_back(neuronValues_[neuronValuesIndex++]);
        }
        std::cout << "\nLW: " << LAYER_WIDTHS[i] << " " << i << "\n\n";
        neuronValuesIndex += LAYER_WIDTHS[i] * LAYER_WIDTHS[i + 1];
    }
    return out;
};