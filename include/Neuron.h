/**
 * \file
 * \author Magdalena Majkowska
 */

#pragma once

#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <iostream>
#include <vector>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

namespace zpr
{
    const unsigned short int NEURONINPUTSNUM = 8;
    const float MUTATIONSTRENGTH = 0.2f;
    const float MUTATIONLOWERBOUND = 0.0f;
    const float MUTATIONUPPERBOUND = 1.0f;
    const float MUTATIONTHRESHHOLD = 0.35f;

    /**
     * \author Magdalena Majkowska
     * 
     * Neuron class, supposed to be used as an abstract for accessing specimen data.
     * It is left unused as CreatureContainer provides faster access to any needed parameters, sadly leaving the project without real data abstraction layer.
     */
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
}; // namespace zpr