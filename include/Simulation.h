#pragma once

#include <vector>
#include <memory>

class Creature;
class Neuron;
class Map;

class Simulation{
    std::vector<Creature> creatures_;
    std::vector<Neuron> neurons_;
    std::unique_ptr<Map> map_;
public:
    virtual ~Simulation();
    Simulation();
};