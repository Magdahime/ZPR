#pragma once

#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <iostream>
#include <vector>
#include <memory>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

class CreatureContainer;

/**
 * \author Magdalena Majkowska
 * 
 * Creature class, supposed to be used as an abstract for accessing specimen data.
 * It is left unused as CreatureContainer provides faster access to any needed parameters, sadly leaving the project without real data abstraction layer.
 */
class Creature
{
    std::weak_ptr<CreatureContainer> creatureContainerPtr_;
    std::vector<unsigned int> layerWidths_;
    size_t neuronBegin_;
    size_t neuronEnd_;
    size_t attributesBegin_;
    size_t attributesEnd_;
    size_t index_;

    Creature() = delete;
    Creature(const Creature &) = delete;
    Creature(Creature &&) = delete;
    Creature &operator=(Creature &&) = delete;
    Creature &operator=(const Creature &) = delete;
public:
    /**
     * Trivial and only constructor for Creature object
     */
    Creature(
        std::weak_ptr<CreatureContainer> creatureContainerPtr,
        const std::vector<unsigned int> &layerWidths,
        size_t neuronBegin,
        size_t neuronEnd,
        size_t attributesBegin,
        size_t attributesEnd,
        size_t index) : creatureContainerPtr_(creatureContainerPtr),
                              layerWidths_(layerWidths),
                              neuronBegin_(neuronBegin),
                              neuronEnd_(neuronEnd),
                              attributesBegin_(attributesBegin),
                              attributesEnd_(attributesEnd),
                              index_(index){};
    /**
     * Method for mutating the Creature
     * \warning Unimplemented, as mutations are processed at CreatureContainer level
     */
    void mutate();
    /**
     * Method to emulate the Creature eating
     * \warning Unimplemented, as eating is processed at CreatureContainer level
     */
    void eat();
    /**
     * Method to emulate reproduction
     * \warning Unimplemented, as reproduction is processed at CreatureContainer level
     */
    void reproduction();
    /**
     * Method to emulate time lapse
     * \warning Unimplemented, as iterations are processed at CreatureContainer level
     */
    void live();
    /**
     * Method to emulate the creature dying
     * \warning Unimplemented, as dying is processed at CreatureContainer level
     */
    void die();
};

using CreatureSPtr = std::shared_ptr<Creature>;