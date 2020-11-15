/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Definition of creature class - basic element  ///
///     of simulation                                 ///
/////////////////////////////////////////////////////////

#ifndef CREATURE
#define CREATURE
#include <iostream>
#include <vector>
#include "neuron.h"

class Creature
{
    std::vector<Neuron>::iterator neuronBegin_;
    std::vector<Neuron>::iterator neuronEnd_;
    float age_;
    float weight_;
    float hunger_;
    float speed_;
    float heading_;
    float posX_;
    float posY_;

public:
    virtual void mutate() = 0;
    virtual void eat() = 0;
    virtual void reproduction() = 0;
    virtual void live() = 0;
    virtual void die() = 0;

    std::vector<Neuron>::iterator Creature::getNeuronBegin()
    {
        return this->neuronBegin_;
    }
    std::vector<Neuron>::iterator Creature::getNeuronEnd()
    {
        return this->neuronEnd_;
    }
    float Creature::getAge()
    {
        return this->age_;
    }
    float Creature::getWeight()
    {
        return this->weight_;
    }
    float Creature::getHunger()
    {
        return this->hunger_;
    }
    float Creature::getSpeed()
    {
        return this->speed_;
    }
    float Creature::getHeading()
    {
        return this->heading_;
    }
    float Creature::getPosX()
    {
        return this->posX_;
    }
    float Creature::getPosY()
    {
        return this->posY_;
    }
    void Creature::setAge(float age)
    {
        this->age_ = age;
    }
    void Creature::setWeight(float weight)
    {
        this->weight_ = weight;
    }
    void Creature::setHunger(float hunger)
    {
        this->hunger_ = hunger;
    }
    void Creature::setSpeed(float speed)
    {
        this->speed_ = speed;
    }
    void Creature::setHeading(float heading)
    {
        this->heading_ = heading;
    }
    void Creature::setPosX(float posX)
    {
        this->posX_ = posX;
    }
    void Creature::setPosY(float posY)
    {
        this->posY_ = posY;
    }
    void Creature::setNeuronBegin(std::vector<Neuron>::iterator neuronBegin)
    {
        this->neuronBegin_ = neuronBegin;
    }
    void Creature::setNeuronEnd(std::vector<Neuron>::iterator neuronEnd)
    {
        this->neuronEnd_ = neuronEnd;
    }
};

#endif