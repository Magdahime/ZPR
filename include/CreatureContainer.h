#pragma once

#pragma warning(push, 0)

#include <vector>
#include <queue>

#include <boost/lockfree/queue.hpp>

#pragma warning(pop)

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

using FullParams = std::tuple<std::shared_ptr<CreatureParameters>, std::shared_ptr<NeuronSet>>;

/**
 * \author Bartłomiej Janowski
 * 
 * CreatureContainer is the main container for simulation data.
 * It stores data of all creatures in stadard library Vectors, but maps unused space from deletions
 * with a Queue, allowing for a decreased memory cost of running simulation for longer.
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
    boost::lockfree::queue<size_t, boost::lockfree::capacity<0>> availableIndexes_;

    std::queue<FullParams> putQueue_;

    std::mutex mutex_;

    unsigned int paramsPerCreature_;
    unsigned int neuronsPerCreature_;
    unsigned int neuronSize_;

    CreatureContainer(const CreatureContainer &) = delete;
    CreatureContainer(CreatureContainer &&) = delete;
    CreatureContainer &operator=(CreatureContainer &&) = delete;
    CreatureContainer &operator=(const CreatureContainer &) = delete;

public:
    void printCapacities() // alert DEBUG
    {
        std::cout << "\ncreatures:\t" << creatureValues_.size() << "\t" << creatureValues_.capacity();
        std::cout << "\nneurons:\t" << neuronValues_.size() << "\t" << neuronValues_.capacity();
        std::cout << "\ntypes:\t" << types_.size() << "\t" << types_.capacity();
    }
    CreatureContainer();
    unsigned int getSize();
    void putCreature(std::string type = DEFAULT_CREATURE);
    NeuronSetSPtr getNeurons(size_t index);
    void putCreature(CreatureParametersSPtr params, NeuronSetSPtr neurons);
    void delayPutCreature(CreatureParametersSPtr params, NeuronSetSPtr neurons);
    void putQueue();
    const size_t getPutQueueSize() { return putQueue_.size(); };
    void allocate(size_t size){
        creatureValues_.reserve(creatureValues_.size() + paramsPerCreature_ * size);
        neuronValues_.reserve(neuronValues_.size() + neuronsPerCreature_ * size);
        types_.reserve(types_.size() + size);
    }
    void updateCreatureParameters(size_t index, CreatureParametersSPtr params);
    const CreatureParametersSPtr getCreatureParameters(size_t index);
    const float getCreatureX(size_t index);
    const float getCreatureY(size_t index);
    void deleteCreature(size_t index);
    bool isDeleted(size_t index);
    void populateNeurons(size_t index);
    void calculateLayer(size_t index, unsigned int layer);
    float getCreatureValue(size_t index, unsigned int value);
    std::vector<float> getResult(size_t index);
    std::vector<std::vector<float>> getNeuronStates(size_t index);
};