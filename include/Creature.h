/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Definition of creature class - basic element  ///
///     of simulation                                 ///
/////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <vector>
#include <memory>
// #include "Neuron.h"

const float MAX_CREATURE_SPEED = 5.0f;
const float MIN_CREATURE_SPEED = 0.0f;
const float MIN_CREATURE_HEADING = 0.0f;
const float MAX_CREATURE_HEADING = 360.0f;

class CreatureContainer;

class Creature
{
    std::weak_ptr<CreatureContainer> creatureContainerPtr_;
    std::vector<unsigned int> layerWidths_;
    unsigned int neuronBegin_;
    unsigned int neuronEnd_;
    unsigned int attributesBegin_;
    unsigned int attributesEnd_;
    unsigned int index_;

public:
    Creature(
        std::weak_ptr<CreatureContainer> creatureContainerPtr,
        const std::vector<unsigned int> &layerWidths,
        unsigned int neuronBegin,
        unsigned int neuronEnd,
        unsigned int attributesBegin,
        unsigned int attributesEnd,
        unsigned int index) : creatureContainerPtr_(creatureContainerPtr),
                              layerWidths_(layerWidths),
                              neuronBegin_(neuronBegin),
                              neuronEnd_(neuronEnd),
                              attributesBegin_(attributesBegin),
                              attributesEnd_(attributesEnd),
                              index_(index){};

    void mutate();
    void eat();
    void reproduction();
    void live();
    void die();
};

using CreatureSPtr = std::shared_ptr<Creature>;