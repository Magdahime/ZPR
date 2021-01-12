/**
 * \file
 * \author Bartłomiej Janowski
 */

#pragma once

namespace zpr
{
    /**
    * \class SimulationParameters
    * This is the structure representing the set of parameters used by Simulation
    */
    struct SimulationParameters
    {
        unsigned int creaturesNum_;
        float energyThreshhold_;
        float minWeight_;
        float weightGained_;
        float weightLost_;
        float birthWeightThreshhold_;
        float energyBirth_;
        float energyBirthFailed_;
        float weightBirth_;
        float birthAgeThreshhold_;
        float anglePerFrame_;
        float accelerationMultiplier_;
        float maxSpeed_;
    };
}; // namespace zpr