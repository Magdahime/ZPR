#ifndef SIMULATION
#define SIMULATION

#include "creature.h"
#include "neuron.h"
#include "vectorSegment.h"

class Simulation
{
    std::vector<Creature> creatures_;
    std::vector<Neuron> neurons_;

    std::vector<float> weights_;
    std::vector<float> inputs_;
    std::vector<float> biases_;

public:
    Simulation() = default;
    VectorSegment<float> newWeightsSegment(int segmentSize)
    {
        VectorSegment<float> vs;
        weights_.resize(weights_.size() + segmentSize);
        vs.segmentFirst = weights_.end() - segmentSize;
        vs.segmentLast = weights_.end() - 1;
        return vs;
    }
    VectorSegment<float> newInputsSegment(int segmentSize)
    {
        VectorSegment<float> vs;
        inputs_.resize(inputs_.size() + segmentSize);
        vs.segmentFirst = inputs_.end() - segmentSize;
        vs.segmentLast = inputs_.end() - 1;
        return vs;
    }
    VectorSegment<float> newBiasesSegment(int segmentSize)
    {
        VectorSegment<float> vs;
        biases_.resize(biases_.size() + segmentSize);
        vs.segmentFirst = biases_.end() - segmentSize;
        vs.segmentLast = biases_.end() - 1;
        return vs;
    }
    VectorSegment<Neuron> newNeuronsSegment(int segmentSize)
    {
        VectorSegment<Neuron> vs;
        neurons_.resize(neurons_.size() + segmentSize);
        vs.segmentFirst = neurons_.end() - segmentSize;
        vs.segmentLast = neurons_.end() - 1;
        return vs;
    }
    void addCreature(Creature creature)
    {
        creatures_.push_back(creature);
    }
    void addCreatures(int num)
    {
        for (int i = 0; i < num; i++)
        {
            auto neuronSegment = newNeuronsSegment(8);
            std::vector<VectorSegment<float>> weightsSegments;
            std::vector<VectorSegment<float>> inputsSegments;
            std::vector<VectorSegment<float>> biasesSegments;
            for (auto j = neuronSegment.segmentFirst; j <= neuronSegment.segmentLast; j++)
            {
                weightsSegments.emplace_back(newWeightsSegment(8));
                inputsSegments.emplace_back(newInputsSegment(8));
                biasesSegments.emplace_back(newBiasesSegment(1));
            }
            creatures_.emplace_back(Creature(neuronSegment, weightsSegments, biasesSegments, inputsSegments, 8, 8));
        }
    }
    std::vector<Neuron>::iterator addNeuron(Neuron neuron)
    {
        neurons_.push_back(neuron);
    }
};

#endif