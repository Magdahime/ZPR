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

#include <vector>
#include <memory>
#include <boost/interprocess/sync/interprocess_semaphore.hpp>

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
const float ACCELERATION_MULTIPLIER = 0.05f;
const float MAX_SPEED = 0.05f;

const float BIRTH_AGE_THRESHOLD = 1.f;

const float TARGET_FPS = 60.f;

const float MIN_WEIGHT = 10.f;


class Creature;
class Neuron;
class Map;

namespace sf
{
    class RenderWindow;
}

struct SimulationParameters{
    unsigned int creaturesNum_;
    float energyThreshhold_;
    float minWeight_;
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
    std::vector<float> data_PROTO_;

    CreatureContainer container_;
    SimulationParameters parameters_;

    void populateNeurons();
    void updateCreature(int creatureIndex);

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
    void setMap(std::shared_ptr<Map> mapPtr);
    void printAll_PROTO(sf::RenderWindow *window);
    void printAll(std::shared_ptr<sf::RenderWindow>);
    void printClipped_PROTO(std::shared_ptr<sf::RenderWindow> window, sf::View view);
    void printClipped(std::shared_ptr<sf::RenderWindow> window, sf::View view);
    void setSimulationParameters(SimulationParameters params) {this->parameters_ = params;}
    SimulationParameters getSimulationParameters(){return parameters_;}
};
