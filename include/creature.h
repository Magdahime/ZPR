/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Definition of creature class - basic element  ///
///     of simulation                                 ///
/////////////////////////////////////////////////////////

#ifndef CREATURE
#define CREATURE
#include "main.h"
#include "neuron.h"

class Creature{
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
    
    void mutate();
    std::vector<Neuron>::iterator getNeuronBegin();
    std::vector<Neuron>::iterator getNeuronEnd();
    float getAge();
    float getWeight();
    float getHunger();
    float getSpeed();
    float getHeading();
    float getPosX();
    float getPosY();
    void setAge(float age);
    void setWeight(float weight);
    void setHunger(float hunger);
    void setSpeed(float speed);
    void setHeading(float heading);
    void setPosX(float posX);
    void setPosY(float posY);
    void setNeuronBegin(std::vector<Neuron>::iterator neuronBegin);
    void setNeuronEnd(std::vector<Neuron>::iterator neuronEnd);
    
};


#endif