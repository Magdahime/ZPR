/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Definition of simple neuron class             ///
/////////////////////////////////////////////////////////

#pragma once

#include <iostream>
#include <vector>

const unsigned short int NEURONINPUTSNUM = 8;
const float MUTATIONSTRENGTH = 0.2f;
const float MUTATIONLOWERBOUND = 0.0f;
const float MUTATIONUPPERBOUND = 1.0f;
const float MUTATIONTHRESHHOLD = 0.35f;

class Neuron
{
    std::vector<float>::iterator inputsBegin_;
    std::vector<float>::iterator inputsEnd_;
    std::vector<float>::iterator weightsBegin_;
    std::vector<float>::iterator weightsEnd_;
    std::vector<float>::iterator bias_;

public:
    Neuron(const std::vector<float>::iterator weightsBegin, const std::vector<float>::iterator weightsEnd);
    float sigmoid(float value);
    void mutate();
    std::vector<float>::iterator getInputsBegin();
    std::vector<float>::iterator getInputsEnd();
    std::vector<float>::iterator getWeightBegin();
    std::vector<float>::iterator getWeightsEnd();
    std::vector<float>::iterator getBias();
    void setBias(std::vector<float>::iterator bias);
    void setInputsBegin(std::vector<float>::iterator inputsBegin);
    void setInputsEnd(std::vector<float>::iterator inputsEnd);
    void setWeightBegin(std::vector<float>::iterator weightsBegin);
    void setWeightsEnd(std::vector<float>::iterator weightsEnd);
};
