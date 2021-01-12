/**
 * \file
 * \author Bartłomiej Janowski
 */

#pragma once

#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <vector>
#include <memory>

#include <boost/interprocess/sync/interprocess_semaphore.hpp>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

#include "Flags.h"
#include "CreatureContainer.h"
#include "structs/SimulationParameters.h"


//Forward declaration
namespace sf
{
    class RenderWindow;
    class View;
} // namespace sf

namespace zpr
{
    const float pi = 3.1415927f;

    const float ACTIVATION_THRESHOLD = 0.8f;

    const float MAX_SPEED = 0.05f;
    const float MIN_SPEED = 0.00001f;

    //Forward declarations
    class Creature;
    class Neuron;
    class Map;

    /**
     * \author Bartłomiej Janowski
     * \class Simulation
     * Simulation class that holds (mostly via CreatureContainer instance) all Simulation data
     */
    class Simulation
    {
        CreatureContainer container_;

        /**
         * Base simulation parameters common to all species used when calculating any data.
         */
        SimulationParameters parameters_;

        std::shared_ptr<Map> map_;

        /**
         * Semaphore at which Simulation thread waits for SFML thread to finish accessing container_
         */
        boost::interprocess::interprocess_semaphore dataSemaphore_;

        /**
         * Semaphore at which Program (SFML) thread waits for Simulation to finish calculations for a frame.
         */
        boost::interprocess::interprocess_semaphore videoSemaphore_;

        unsigned int iterationNumber_ = 0;
        size_t populationSize_ = 0;
        float avgWeight_ = 0.f;
        float avgAge_ = 0.f;

        bool terminate_ = false;

        /**
         * Method for calculating passing of a single frame for the set creature
         */
        void updateCreature(size_t creatureIndex);

        /**
         * Method for finding and selecting the closest creature to a set point.
         */
        void findClosestCreature(float x, float y);

        size_t selectedIndex_ = 0 - 1;

        /**
         * Mutex guarding for only one iteration to run at the same time
         */
        std::mutex iterationMutex_;

        Simulation(const Simulation &) = delete;
        Simulation(Simulation &&) = delete;
        Simulation &operator=(Simulation &&) = delete;
        Simulation &operator=(const Simulation &) = delete;

    public:
        Simulation();
        ~Simulation() = default;

        /**
         * Spawns initial default species.
         */
        void prepare();

        /**
         * Spawns creatureCount of default species.
         */
        void prepare(size_t creatureCount);

        /**
         * Puts creatureNum of creautures as specified by type (if registered in CreatureFactory)
         */
        void putCreature(std::string type, int creatureNum = 1);

        /**
         * Starts the simulation run, internally running iteration() in infinite loop.
         */
        void run();

        /**
         * Runs a single iteration of the simulation on the exisiting container_.
         */
        int iteration();

        /**
         * Informs the run() thread that rendering has been finished and container_ can be accessed again.
         * Internally performs post() at dataSemaphore_
         */
        void postVideo();

        /**
         * Waits at dataSemaphore_ for iteration() to finish
         */
        void waitVideo();

        /**
         * Calculates and performs steer (change of heading) for the creature based on NN result. 
         */
        void calculateSteer(CreatureParametersSPtr creature, float result);

        /**
         * Calculates and performs steer (change of speed) for the creature based on NN result. 
         */
        void calculateAcceleration(CreatureParametersSPtr creature, float result);

        /**
         * Calculates and performs eating for the creature based on NN result. 
         */
        void calculateEating(CreatureParametersSPtr creature, float result);

        /**
         * Calculates and performs attack for the creature based on NN result. 
         * \note
         * Unimplemented
         */
        void calculateAttack(CreatureParametersSPtr creature, float result);

        /**
         * Calculates and, if applicable, performs delayed birth for the creature based on NN result.
         */
        bool calculateBirth(CreatureParametersSPtr creature, float result);

        /**
         * Calculates and performs movement for the creature based on NN result.
         */
        void calculateMovement(CreatureParametersSPtr creature);

        /**
         * Retrieves Hue values from map_ and populates data slots for creature's antennae.
         */
        void calculateAntennas(CreatureParametersSPtr creature);

        /**
         * Increases the creature age based on simulation settings.
         */
        void calculateAge(CreatureParametersSPtr creature);

        /**
         * Calculates remaining energy for a creature based on current value an weight.
         */
        void calculateEnergy(CreatureParametersSPtr creature);

        /**
         * Prints all non-deleted creatures from container_ onto window.
         */
        void printAll(std::shared_ptr<sf::RenderWindow> window);

        /**
         * Prints all non-deleted creatures that fit in view from container_ onto window.
         */
        void printClipped(std::shared_ptr<sf::RenderWindow> window, sf::View view);

        void setMap(std::shared_ptr<Map> mapPtr);
        void setSimulationParameters(SimulationParameters params) { this->parameters_ = params; };
        SimulationParameters getSimulationParameters() { return parameters_; };

        /**
         * Set the creature closest to set cooridinates as the selected one (if exists).
         * Internally calls findClosestCreature() on a new thread to minimize overhead.
         */
        void selectClosestCreature(float x, float y);
        float getSelectedX();
        float getSelectedY();

        /**
         * Informs whether a creature is selected at the moment
         */
        bool isSelected() { return (selectedIndex_ != static_cast<size_t>(0 - 1)); };

        /**
         * Returns a JSON string of selected creature's parameters as accepted by JS frontend.
         */
        std::string getSelectedParametersAsJSON();

        /**
         * Returns a JSON string of selected creature's Neural Network values as accepted by JS frontend.
         */
        std::string getSelectedNeuronsAsJSON();

        /**
         * Unselects the selected creature.
         */
        void unselect();

        inline float getSimulationSecond() { return (iterationNumber_ / zpr_windows::TARGET_FPS); };
        inline size_t getPopulationSize() { return populationSize_; };
        inline float getAvgWeight() { return avgWeight_; };
        inline float getAvgAge() { return avgAge_; };

        /**
         * Unlocks SFML thread if waiting for simulation data.
         * Useful on termination
         */
        void unlockSFML()
        {
            videoSemaphore_.post();
        }

        /**
         * Allows for clean termination of thread running run()
         */
        void terminate()
        {
            terminate_ = true;
            dataSemaphore_.post();
            videoSemaphore_.post();
        };
    };
}; // namespace zpr