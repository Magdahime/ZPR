/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Source code of creature class - basic element ///
///     of simulation                                 ///
/////////////////////////////////////////////////////////
#include "main.h"
#include "creature.h"
#include "neuron.h"

    void Creature::mutate(){
        for(auto i = this->neuronBegin_; i != neuronEnd_; i++){
            (*i).mutate();
        }
    }
    std::vector<Neuron>::iterator Creature::getNeuronBegin(){
        return this->neuronBegin_;
    }
    std::vector<Neuron>::iterator Creature::getNeuronEnd(){
        return this->neuronEnd_;
    }
    float Creature::getAge(){
        return this->age_;
    }
    float Creature::getWeight(){
        return this->weight_;
    }
    float Creature::getHunger(){
        return this->hunger_;
    }
    float Creature::getSpeed(){
        return this->speed_;
    }
    float Creature::getHeading(){
        return this->heading_;
    }
    float Creature::getPosX(){
        return this->posX_;
    }
    float Creature::getPosY(){
        return this->posY_;
    }
    void Creature::setAge(float age){
        this->age_=age;
    }
    void Creature::setWeight(float weight){
        this->weight_= weight;
    }
    void Creature::setHunger(float hunger){
        this->hunger_=hunger;
    }
    void Creature::setSpeed(float speed){
        this->speed_=speed;
    }
    void Creature::setHeading(float heading){
        this->heading_=heading;
    }
    void Creature::setPosX(float posX){
        this->posX_=posX;
    }
    void Creature::setPosY(float posY){
        this->posY_=posY;
    }
    void Creature::setNeuronBegin(std::vector<Neuron>::iterator neuronBegin){
        this->neuronBegin_=neuronBegin;
    }
    void Creature::setNeuronEnd(std::vector<Neuron>::iterator neuronEnd){
        this->neuronEnd_=neuronEnd;
    }