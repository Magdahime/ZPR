/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Definition of creature class - basic element  ///
///     of simulation                                 ///
/////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "Neuron.h"

const float MAX_CREATURE_SPEED = 5.0f;
const float MIN_CREATURE_SPEED = 0.0f;
const float MIN_CREATURE_HEADING = 0.0f;
const float MAX_CREATURE_HEADING = 360.0f;

class Creature
{
    std::weak_ptr<std::vector<float>> neuronVector_;
    std::weak_ptr<std::vector<float>> creatureVector_;

    unsigned int neuronBegin_;
    unsigned int neuronEnd_;
    unsigned int attributesBegin_;
    unsigned int attributesEnd_;

public:
    Creature(
        std::shared_ptr<std::vector<float>> neuronVector,
        std::shared_ptr<std::vector<float>> creatureVector,
        unsigned int neuronBegin,
        unsigned int neuronEnd,
        unsigned int attributesBegin,
        unsigned int attributesEnd) : neuronVector_(neuronVector),
                                      creatureVector_(creatureVector),
                                      neuronBegin_(neuronBegin),
                                      neuronEnd_(neuronEnd),
                                      attributesBegin_(attributesBegin),
                                      attributesEnd_(attributesEnd){};

    virtual void mutate() = 0;
    virtual void eat() = 0;
    virtual void reproduction() = 0;
    virtual void live() = 0;
    virtual void die() = 0;
};

class PeacefulCreature : Creature
{

public:
    static Creature *create();
};

class AggressiveCreature : Creature
{

public:
    static Creature *create();
};

using CreatureSPtr = std::shared_ptr<Creature>;