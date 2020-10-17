/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Source code of simple neuron class            ///
/////////////////////////////////////////////////////////

#include "main.h"
#include "neuron.h"

float Neuron::sigmoid(float value){
    return value/(1+abs(value));
}
std::vector<float>::iterator Neuron::getBias() {
    return this.bias_;
}
std::vector<float>::iterator Neuron::getInputsBegin() {
    return this.inputsBegin_;
}

std::vector<float>::iterator Neuron::getInputsEnd() {
    return this.inputsEnd_;
}
std::vector<float>::iterator Neuron::getWeightBegin() {
    return this.weightsBegin_;
}
std::vector<float>::iterator Neuron::getWeightsEnd() {
    return this.weightsEnd_;
}
void Neuron::setBias(std::vector<float>::iterator bias) {
    this.bias = bias;
}
void Neuron::setInputsBegin(std::vector<float>::iterator inputsBegin) {
    this.inputsBegin = inputsBegin;
}
void Neuron::setInputsEnd(std::vector<float>::iterator inputsEnd) {
    this.inputsEnd = inputsEnd;
}
void Neuron::setWeightBegin(std::vector<float>::iterator weightsBegin) {
    this.weightsBegin = weightsBegin;
}
void Neuron::setWeightsEnd(std::vector<float>::iterator weightsEnd) {
    this.weightsEnd = weightsEnd;
}
