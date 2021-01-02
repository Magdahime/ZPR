/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Definition of CreaturesContainer - that will  ///
///     store all of the values of creatures so that  ///
///     open MP can calculate them                    ///
/////////////////////////////////////////////////////////

#pragma once
#include <vector>
#include "Creature.h"

class CreatureContainer{

    std::vector<float> creatureContainer;

public:
    CreatureContainer(int populationNumber, int attributesNumber);
    void deleteCreature(unsigned int index);
};