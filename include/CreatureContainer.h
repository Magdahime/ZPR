#pragma once

#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <vector>
#include <queue>

#include <boost/lockfree/queue.hpp>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

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

    /**
     * Mutex at which inputting creatures is synchronised
     */
    std::mutex mutex_;

    unsigned int paramsPerCreature_;
    unsigned int neuronsPerCreature_;
    unsigned int neuronSize_;

    /**
     * Following values are used for normalization of input values in the neural network. 
     * As MSVC does not allow for reducing max/min functions in OpenMP loops, we are normalizing
     * linearly with average value as 0.
     * 
     * Default values only hold for the first frame of simulation, when the respective values are not
     * yet known. 
     */
    float energyDivisor_ = 1.f;
    float weightDivisor_ = 1.f;
    float ageDivisor_ = 1.f;
    float speedDivisor_ = 1.f;

    CreatureContainer(const CreatureContainer &) = delete;
    CreatureContainer(CreatureContainer &&) = delete;
    CreatureContainer &operator=(CreatureContainer &&) = delete;
    CreatureContainer &operator=(const CreatureContainer &) = delete;

public:
    CreatureContainer();
    size_t getSize();
    void putCreature(std::string type = DEFAULT_CREATURE);
    const CreatureParametersSPtr getCreatureParameters(size_t index);
    NeuronSetSPtr getNeurons(size_t index);

    /**
     * Method that puts creature defined by params and neurons into the container
     * \note
     * May wait at mutex_
     */
    void putCreature(CreatureParametersSPtr params, NeuronSetSPtr neurons);
    
    /**
     * Method that puts and neurons into the putQueue_, with the putting into the container delayed
     * until putQueue()
     */
    void delayPutCreature(CreatureParametersSPtr params, NeuronSetSPtr neurons);

    /**
     * Method putting all the entries from putQueue_ into the container
     */
    void putQueue();

    size_t getPutQueueSize() { return putQueue_.size(); };

    /**
     * Method replacing creatureValues_ entry for creature at index with values from params
     */
    void updateCreatureParameters(size_t index, CreatureParametersSPtr params);

    float getCreatureX(size_t index);
    float getCreatureY(size_t index);

    void deleteCreature(size_t index);
    bool isDeleted(size_t index);

    /**
     * Method populating neurons (i.e. copying and normalizing values from creatureValues_)
     * for the creature at set index if applicable (in bound and not deleted).
     */
    void populateNeurons(size_t index);
    
    /**
     * Method calculating neuron outputs for the set creature at set layer.
     */
    void calculateLayer(size_t index, unsigned int layer);

    /**
     * Method returning n'th value from creatureValues_ for the creature at index
     * \param value - position of the value to be retrieved (0 to paramsPerCreature_)
     */
    float getCreatureValue(size_t index, unsigned int value);

    /**
     * Method for retrieving the result (i.e. output of the last layer of the NN) of Neural Network
     * for the set creature.
     */
    std::vector<float> getResult(size_t index);

    /**
     * Method for retrieving of all layers of of Neural Network for the set creature.
     */
    std::vector<std::vector<float>> getNeuronStates(size_t index);

    void setDivisors(float energyDivisor, float weightDivisor, float ageDivisor, float speedDivisor){
        energyDivisor_ = energyDivisor;
        weightDivisor_ = weightDivisor;
        ageDivisor_ = ageDivisor;
        speedDivisor_ = speedDivisor;
    }
};