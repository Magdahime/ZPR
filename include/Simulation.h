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

class Simulation
{
    std::vector<Creature> creatures_;
    std::vector<Neuron> neurons_;
    std::vector<float> weights_;
    std::shared_ptr<Map> map_;
    boost::interprocess::interprocess_semaphore dataSemaphore_;
    boost::interprocess::interprocess_semaphore videoSemaphore_;
    std::vector<float> data_PROTO_;

    CreatureContainer container_;
    SimulationParameters parameters_;

    unsigned int iterationNumber_ = 0;
    unsigned int populationSize_ = 0;
    float totalWeight_ = 0.f;
    float avgAge_ = 0.f;

    bool terminate_ = false;

    void populateNeurons();
    void updateCreature(int creatureIndex);

    void findClosestCreature(float x, float y);

    unsigned int selectedIndex_ = 0 - 1;

    std::mutex iterationMutex_;

    Simulation(const Simulation &) = delete;
    Simulation(Simulation &&) = delete;
    Simulation &operator=(Simulation &&) = delete;
    Simulation &operator=(const Simulation &) = delete;

public:
    virtual ~Simulation();
    Simulation();
    void prepare();
    void prepare(unsigned int creatureCount);
    void run();
    int iteration();
    void run_PROTO();
    void iteration_PROTO();
    bool tryNewData();
    void postVideo();
    void waitVideo();

    void calculateSteer(CreatureParametersSPtr creature, float result);
    void calculateAcceleration(CreatureParametersSPtr creature, float result);
    void calculateEating(CreatureParametersSPtr creature, float result);
    void calculateAttack(CreatureParametersSPtr creature, float result);
    bool calculateBirth(CreatureParametersSPtr creature, float result);
    void calculateMovement(CreatureParametersSPtr creature);
    void calculateAntennas(CreatureParametersSPtr creature);
    void calculateAge(CreatureParametersSPtr creature);
    void calculateEnergy(CreatureParametersSPtr creature);

    void setMap(std::shared_ptr<Map> mapPtr);
    void printAll(std::shared_ptr<sf::RenderWindow>);
    void printClipped(std::shared_ptr<sf::RenderWindow> window, sf::View view);
    void setSimulationParameters(SimulationParameters params) { this->parameters_ = params; };

    void selectClosestCreature(float x, float y);

    inline float getSimulationSecond() { return (iterationNumber_ / TARGET_FPS); };
    inline unsigned int getPopulationSize() { return populationSize_; };
    inline float getTotalWeight() { return totalWeight_; };
    inline float getAvgAge() { return avgAge_; };

    bool isSelected() { return (selectedIndex_ != 0 - 1); };
    float getSelectedX();
    float getSelectedY();

    std::string getSelectedParametersAsJSON();
    std::string getSelectedNeuronsAsJSON();

    SimulationParameters getSimulationParameters() { return parameters_; };

    void terminate()
    {
        terminate_ = true;
        dataSemaphore_.post();
        videoSemaphore_.post();
        dataSemaphore_.post();
        videoSemaphore_.post();
        dataSemaphore_.post();
        videoSemaphore_.post();
        dataSemaphore_.post();
        videoSemaphore_.post();
        dataSemaphore_.post();
        videoSemaphore_.post();
    };

    void putCreature(std::string type, int creatureNum = 1);
};
