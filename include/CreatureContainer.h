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

const std::string DELETED_DESIGNATOR = "__DEL__"; 

class CreatureContainer
{

    std::vector<float> creatureValues_;
    std::vector<float> neuronValues_;
    std::vector<Neuron> neurons_;
    std::vector<Creature> creatures_;
    std::vector<std::string> types_;

    std::queue<unsigned int> availableIndexes_;

    std::mutex mutex;


    unsigned int paramsPerCreature_;
    unsigned int neuronsPerCreature_;
    unsigned int neuronSize_;

public:
    CreatureContainer(
        unsigned int paramsPerCreature,
        unsigned int neuronsPerCreature) : paramsPerCreature_(paramsPerCreature),
                                           neuronsPerCreature_(neuronsPerCreature){};

    void putCreature(const CreatureParameters &params, const std::vector<Neuron> &neurons){
        std::lock_guard<std::mutex> lockGuard(mutex); 
    };
    const CreatureParametersSPtr getCreatureParameters(unsigned int index)
    {
        std::lock_guard<std::mutex> lockGuard(mutex);
        CreatureParametersSPtr parameters = std::make_shared<CreatureParameters>(CreatureParameters {
            types_[index],
            creatureValues_[index * paramsPerCreature_ + 0],
            creatureValues_[index * paramsPerCreature_ + 1],
            creatureValues_[index * paramsPerCreature_ + 2],
            creatureValues_[index * paramsPerCreature_ + 3],
            creatureValues_[index * paramsPerCreature_ + 4],
            creatureValues_[index * paramsPerCreature_ + 5],
            creatureValues_[index * paramsPerCreature_ + 6],
            creatureValues_[index * paramsPerCreature_ + 7],
            creatureValues_[index * paramsPerCreature_ + 8],
            creatureValues_[index * paramsPerCreature_ + 9],
            creatureValues_[index * paramsPerCreature_ + 10],
            creatureValues_[index * paramsPerCreature_ + 11],
            creatureValues_[index * paramsPerCreature_ + 12]
        });
        return parameters;
    };
    void deleteCreature(unsigned int index){
        std::lock_guard<std::mutex> lockGuard(mutex);
        try {
            if(types_[index] == DELETED_DESIGNATOR){
                throw std::exception();
            }
        } catch (std::exception ex){
            return;
        }
        types_[index] = DELETED_DESIGNATOR;
        availableIndexes_.push(index);
    };
};