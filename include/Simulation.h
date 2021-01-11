/////////////////////////////////////////////////////////
///     Author: Bartlomiej Janowski                   ///
///     Simulation class that holds all necessary     ///
///     simulation data.                              ///
///                                                   ///
///     As you can see there are some "_PROTO"        ///
///     members. Those will only be used as a         ///
///     "proof-of-concept" on library and runtime     ///
///     cross-compatibility.                          ///
///                                                   ///
/////////////////////////////////////////////////////////
#pragma once

#pragma warning (push, 0)

#include <vector>
#include <memory>

#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#pragma warning (pop)

#include "CreatureContainer.h"

const unsigned int CREATURE_COUNT_PROTO = 10;
const float pi = 3.1415927f;

const float ACTIVATION_THRESHOLD = 0.8f;

const float ENERGY_THRESHOLD = 100.f;
const float BIRTH_WEIGHT_THRESHOLD = 100.f;
const float ENERGY_BIRTH = 20.f;
const float ENERGY_BIRTH_FAILED = 20.f;
const float WEIGHT_BIRTH = 20.f;

const float ANGLE_PER_FRAME = 2.0f;
const float ACCELERATION_MULTIPLIER = 1.05f;
const float MAX_SPEED = 0.05f;
const float MIN_SPEED = 0.00001f;

const float BIRTH_AGE_THRESHOLD = 1.f;

const float TARGET_FPS = 60.f;

const float MIN_WEIGHT = 10.f;

class Creature;
class Neuron;
class Map;

namespace sf
{
    class RenderWindow;
    class View;
} // namespace sf

struct SimulationParameters
{
    unsigned int creaturesNum_;
    float energyThreshhold_;
    float minWeight_;
    float weightGained_;
    float weightLost_;
    float birthWeightThreshhold_;
    float energyBirth_;
    float energyBirthFailed_;
    float weightBirth_;
    float birthAgeThreshhold_;
    float anglePerFrame_;
    float accelerationMultiplier_;
    float maxSpeed_;
};
/**
 * Simulation class that holds (mostly via CreatureContainer instance) all Simulation data
 */
class Simulation
{
    CreatureContainer container_;
    
    /**
     * Base simulation parameters common to all species used when calculating any data.
     */
    SimulationParameters parameters_;

    std::shared_ptr<Map> map_;

    /**
     * Semaphore at which Simulation thread waits for SFML thread to finish accessing container_
     */
    boost::interprocess::interprocess_semaphore dataSemaphore_;

    /**
     * Semaphore at which Program (SFML) thread waits for Simulation to finish calculations for a frame.
     */
    boost::interprocess::interprocess_semaphore videoSemaphore_;

    unsigned int iterationNumber_ = 0;
    unsigned int populationSize_ = 0;
    float avgWeight_ = 0.f;
    float avgAge_ = 0.f;

    bool terminate_ = false;

    /**
     * Method for calculating passing of a single frame for the set creature
     */
    void updateCreature(int creatureIndex);

    /**
     * Method for finding and selecting the closest creature to a set point.
     */
    void findClosestCreature(float x, float y);

    unsigned int selectedIndex_ = 0 - 1;

    /**
     * Mutex guarding for only one iteration to run at the same time
     */
    std::mutex iterationMutex_;

    Simulation(const Simulation &) = delete;
    Simulation(Simulation &&) = delete;
    Simulation &operator=(Simulation &&) = delete;
    Simulation &operator=(const Simulation &) = delete;

public:
    Simulation();
    ~Simulation() = default;
    
    /**
     * Spawns initial default species.
     */
    void prepare();

    /**
     * Spawns creatureCount of default species.
     */
    void prepare(unsigned int creatureCount);

    /**
     * Puts creatureNum of creautures as specified by type (if registered in CreatureFactory)
     */
    void putCreature(std::string type, int creatureNum = 1);

    /**
     * Starts the simulation run, internally running iteration() in infinite loop.
     */
    void run();

    /**
     * Runs a single iteration of the simulation on the exisiting container_.
     */
    int iteration();
    
    /**
     * Informs the run() thread that rendering has been finished and container_ can be accessed again.
     * Internally performs post() at dataSemaphore_
     */
    void postVideo();

    /**
     * Waits at dataSemaphore_ for iteration() to finish
     */
    void waitVideo();

    /**
     * Calculates and performs steer (change of heading) for the creature based on NN result. 
     */
    void calculateSteer(CreatureParametersSPtr creature, float result);

    /**
     * Calculates and performs steer (change of speed) for the creature based on NN result. 
     */
    void calculateAcceleration(CreatureParametersSPtr creature, float result);

    /**
     * Calculates and performs eating for the creature based on NN result. 
     */
    void calculateEating(CreatureParametersSPtr creature, float result);

    /**
     * Calculates and performs attack for the creature based on NN result. 
     * \note
     * Unimplemented
     */
    void calculateAttack(CreatureParametersSPtr creature, float result);

    /**
     * Calculates and, if applicable, performs delayed birth for the creature based on NN result.
     */
    bool calculateBirth(CreatureParametersSPtr creature, float result);

    /**
     * Calculates and performs movement for the creature based on NN result.
     */
    void calculateMovement(CreatureParametersSPtr creature);

    /**
     * Retrieves Hue values from map_ and populates data slots for creature's antennae.
     */
    void calculateAntennas(CreatureParametersSPtr creature);

    /**
     * Increases the creature age based on simulation settings.
     */
    void calculateAge(CreatureParametersSPtr creature);

    /**
     * Calculates remaining energy for a creature based on current value an weight.
     */
    void calculateEnergy(CreatureParametersSPtr creature);

    /**
     * Prints all non-deleted creatures from container_ onto window.
     */
    void printAll(std::shared_ptr<sf::RenderWindow> window);

    /**
     * Prints all non-deleted creatures that fit in view from container_ onto window.
     */
    void printClipped(std::shared_ptr<sf::RenderWindow> window, sf::View view);

    void setMap(std::shared_ptr<Map> mapPtr);
    void setSimulationParameters(SimulationParameters params) { this->parameters_ = params; };
    SimulationParameters getSimulationParameters() { return parameters_; };

    /**
     * Set the creature closest to set cooridinates as the selected one (if exists).
     * Internally calls findClosestCreature() on a new thread to minimize overhead.
     */
    void selectClosestCreature(float x, float y);
    const float getSelectedX();
    const float getSelectedY();

    /**
     * Informs whether a creature is selected at the moment
     */
    bool isSelected() { return (selectedIndex_ != 0 - 1); };

    /**
     * Returns a JSON string of selected creature's parameters as accepted by JS frontend.
     */
    std::string getSelectedParametersAsJSON();

    /**
     * Returns a JSON string of selected creature's Neural Network values as accepted by JS frontend.
     */
    std::string getSelectedNeuronsAsJSON();


    inline float getSimulationSecond() { return (iterationNumber_ / TARGET_FPS); };
    inline unsigned int getPopulationSize() { return populationSize_; };
    inline float getAvgWeight() { return avgWeight_; };
    inline float getAvgAge() { return avgAge_; };


    /**
     * Unlocks SFML thread if waiting for simulation data.
     * Useful on termination
     */
    void unlockSFML(){
        videoSemaphore_.post();
    }

    /**
     * Allows for clean termination of thread running run()
     */
    void terminate()
    {
        terminate_ = true;
        dataSemaphore_.post();
        videoSemaphore_.post();
    };
};
