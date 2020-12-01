/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Definition of creature class - basic element  ///
///     of simulation                                 ///
/////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <vector>
#include "Neuron.h"

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

    std::vector<Neuron>::iterator getNeuronBegin()
    {
        return this->neuronBegin_;
    }
    std::vector<Neuron>::iterator getNeuronEnd()
    {
        return this->neuronEnd_;
    }
    float getAge()
    {
        return this->age_;
    }
    float getWeight()
    {
        return this->weight_;
    }
    float getHunger()
    {
        return this->hunger_;
    }
    float getSpeed()
    {
        return this->speed_;
    }
    float getHeading()
    {
        return this->heading_;
    }
    float getPosX()
    {
        return this->posX_;
    }
    float getPosY()
    {
        return this->posY_;
    }
    void setAge(float age)
    {
        this->age_ = age;
    }
    void setWeight(float weight)
    {
        this->weight_ = weight;
    }
    void setHunger(float hunger)
    {
        this->hunger_ = hunger;
    }
    void setSpeed(float speed)
    {
        this->speed_ = speed;
    }
    void setHeading(float heading)
    {
        this->heading_ = heading;
    }
    void setPosX(float posX)
    {
        this->posX_ = posX;
    }
    void setPosY(float posY)
    {
        this->posY_ = posY;
    }
    void setNeuronBegin(std::vector<Neuron>::iterator neuronBegin)
    {
        this->neuronBegin_ = neuronBegin;
    }
    void setNeuronEnd(std::vector<Neuron>::iterator neuronEnd)
    {
        this->neuronEnd_ = neuronEnd;
    }
};
