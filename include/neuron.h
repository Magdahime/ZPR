/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Definition of simple neuron class             ///
/////////////////////////////////////////////////////////

#ifndef NEURON
#define NEURON
#include "main.h"

const unsigned short int NEURONINPUTSNUM;
const float MUTATIONPERCENTAGE;

class Neuron{
    const std::vector<float>::iterator inputsBegin_;
    const std::vector<float>::iterator inputsEnd_;
    const std::vector<float>::iterator weightsBegin_;
    const std::vector<float>::iterator weightsEnd_;
    const std::vector<float>::iterator bias_;
public:
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

}
#endif