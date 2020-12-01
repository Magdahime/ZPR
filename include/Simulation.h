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

const unsigned int CREATURE_COUNT_PROTO = 1000;
const float pi = 3.1415927f;

class Creature;
class Neuron;
class Map;

namespace sf
{
    class RenderWindow;
}

class Simulation
{
    std::vector<Creature> creatures_;
    std::vector<Neuron> neurons_;
    std::vector<float> weights_;
    std::shared_ptr<Map> map_;
    boost::interprocess::interprocess_semaphore dataSemaphore_;
    std::vector<float> data_PROTO_;
    void populateNeurons();

public:
    virtual ~Simulation();
    Simulation();
    void run();
    void iteration();
    void run_PROTO();
    void iteration_PROTO();
    bool tryNewData();
    void setMap(std::shared_ptr<Map> mapPtr);
    void printAll_PROTO(sf::RenderWindow *window);
};