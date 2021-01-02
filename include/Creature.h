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
    std::vector<Neuron>::iterator neuronBegin_;
    std::vector<Neuron>::iterator neuronEnd_;
    std::vector<float>::iterator attributesBegin_;
    std::vector<float>::iterator attributesEnd_;

public:

    virtual void mutate() = 0;
    virtual void eat() = 0;
    virtual void reproduction() = 0;
    virtual void live() = 0;
    virtual void die() = 0;
};


class PeacefulCreature : Creature{

public:
    static Creature * create();
};

class AggressiveCreature : Creature{

public:
    static Creature * create();
};

using CreatureSPtr = std::shared_ptr<Creature>;