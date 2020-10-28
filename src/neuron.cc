/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Source code of simple neuron class            ///
/////////////////////////////////////////////////////////

#include "main.h"
#include "neuron.h"
#include "randomNumberGenerator.h"

float Neuron::sigmoid(float value){
    return value/(1+abs(value));
}
void Neuron::mutate(){
    RandomNumberGenerator<float> rng(MUTATIONLOWERBOUND, MUTATIONUPPERBOUND);
    for(auto i = weightsBegin_; i != weightsEnd_; i++ ){
        if(rng.get() <= MUTATIONTHRESHHOLD){
            *i = rng.getNormal((*i),MUTATIONSTRENGTH * (*i)); 
        }
    }
}
std::vector<float>::iterator Neuron::getBias() {
    return this->bias_;
}
std::vector<float>::iterator Neuron::getInputsBegin() {
    return this->inputsBegin_;
}

std::vector<float>::iterator Neuron::getInputsEnd() {
    return this->inputsEnd_;
}
std::vector<float>::iterator Neuron::getWeightBegin() {
    return this->weightsBegin_;
}
std::vector<float>::iterator Neuron::getWeightsEnd() {
    return this->weightsEnd_;
}
void Neuron::setBias(std::vector<float>::iterator bias) {
    this->bias_ = bias;
}
void Neuron::setInputsBegin(std::vector<float>::iterator inputsBegin) {
    this->inputsBegin_ = inputsBegin;
}
void Neuron::setInputsEnd(std::vector<float>::iterator inputsEnd) {
    this->inputsEnd_ = inputsEnd;
}
void Neuron::setWeightBegin(std::vector<float>::iterator weightsBegin) {
    this->weightsBegin_ = weightsBegin;
}
void Neuron::setWeightsEnd(std::vector<float>::iterator weightsEnd) {
    this->weightsEnd_ = weightsEnd;
}
