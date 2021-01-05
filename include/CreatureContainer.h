/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska,                  ///
///             Bartłomiej Janowski                   ///
///     Definition of CreaturesContainer - that will  ///
///     store all of the values of creatures so that  ///
///     OpenMP can calculate them                     ///
/////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include <queue>
#include "Creature.h"
#include "CreatureFactory.h"
#include "NeuronFactory.h"

const std::string DELETED_DESIGNATOR = "__DEL__";

class CreatureContainer : public std::enable_shared_from_this<CreatureContainer>
{

    std::vector<float> creatureValues_;
    std::vector<float> neuronValues_;
    // std::vector<Neuron> neurons_;
    std::vector<Creature> creatures_;
    std::vector<std::string> types_;

    std::queue<unsigned int> availableIndexes_;

    std::mutex mutex;

    unsigned int paramsPerCreature_;
    unsigned int neuronsPerCreature_;
    unsigned int neuronSize_;

public:
    CreatureContainer();
    unsigned int getSize();
    void putCreature();
    NeuronSetSPtr getNeurons(unsigned int index);
    void putCreature(CreatureParametersSPtr params, NeuronSetSPtr neurons);
    void updateCreatureParameters(unsigned int index, CreatureParametersSPtr params);
    const CreatureParametersSPtr getCreatureParameters(unsigned int index);
    const float getCreatureX(unsigned int index);
    const float getCreatureY(unsigned int index);
    void deleteCreature(unsigned int index);
    bool isDeleted(unsigned int index);
    void populateNeurons(unsigned int index);
    void calculateLayer(unsigned int index, unsigned int layer);
    std::vector<float> getResult(int index);
};