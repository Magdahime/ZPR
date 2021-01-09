#pragma once

#pragma warning (push, 0)

#include <vector>
#include <queue>

#pragma warning (pop)

#include "Creature.h"
#include "CreatureFactory.h"
#include "NeuronFactory.h"

/**
 * Designator used as the type of any deleted creature in the types_ vector.
 * It is needed to ommit those creatures when calculating the values of simulation.
 */
const std::string DELETED_DESIGNATOR = "__DEL__";

const char CONTAINER_SLOT_WEIGHT = 1;
const char CONTAINER_SLOT_AGE = 6;
/**
 * \author Bartłomiej Janowski
 * 
 * CreatureContainer is the main container for simulation data.
 * It stores data of all creatures in stadard Vectors, but maps unused space from deletions
 * with a Queue, allowing for decreased memory cost of running simulation for longer.
 * 
 * \note 
 * The design of this class, i.e. the usage of plain float value vectors stems from the initial
 * idea of using OpenCL for the calculations. While it would be beneficial for OpenCL calculations,
 * the lack of an abstraction layer of a creature proved to be an issue in the rest of the program.
 */
class CreatureContainer : public std::enable_shared_from_this<CreatureContainer>
{

    std::vector<float> creatureValues_;
    std::vector<float> neuronValues_;
    // std::vector<Neuron> neurons_;
    // std::vector<Creature> creatures_;
    std::vector<std::string> types_;
    /**
     * Queue of the indexes that are allocated in the vector, but remain free
     * as they were emptied by dead/removed creatures
     */
    std::queue<unsigned int> availableIndexes_;

    std::mutex mutex;

    unsigned int paramsPerCreature_;
    unsigned int neuronsPerCreature_;
    unsigned int neuronSize_;

    CreatureContainer(const CreatureContainer &) = delete;
    CreatureContainer(CreatureContainer &&) = delete;
    CreatureContainer &operator=(CreatureContainer &&) = delete;
    CreatureContainer &operator=(const CreatureContainer &) = delete;

public:
    CreatureContainer();
    unsigned int getSize();
    void putCreature(std::string type = DEFAULT_CREATURE);
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
    float getCreatureValue(unsigned int index, unsigned int value);
    std::vector<float> getResult(int index);
    std::vector<std::vector<float>> getNeuronStates(int index);
};