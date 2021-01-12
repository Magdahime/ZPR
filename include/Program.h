/**
 * \file
 * \author Bartłomiej Janowski
 */

#pragma once

#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

//C++ Standard Library stuff.
#include <memory>
#include <string>
#include <thread>

//SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

// ZPR
#include "Simulation.h"

//Forward declaration
namespace webview
{
    class webview;
}

namespace zpr
{
    //Forward declaration
    class PyWebserver;

    /**
     * \author Bartłomiej Janowski
     * \class Program
     * 
     * This is the main class of the Program. It stores all needed pointers to other segments of the app.
     * 
     * Program also manages the threads that are used across the application.
     */
    class Program
    {
    public:
        struct SimulationData
        {
            float secondNum;
            unsigned int populationSize;
            float avgWeight;
            float avgAge;
        };

    private:
        std::shared_ptr<sf::RenderWindow> programWindowPtr_;
        std::shared_ptr<webview::webview> webviewPtr_;
        std::shared_ptr<Simulation> simulationPtr_;
        std::shared_ptr<PyWebserver> webserverPtr_;
        std::thread webviewThread_;
        std::thread simulationThread_;
        std::thread webserverThread_;
        std::thread statisticsThread_;
        SimulationData statistics_;

        /**
         * Semaphore at which the program waits until webview is initialised.
         */
        boost::interprocess::interprocess_semaphore webviewSemaphore_;

        /**
         * Semaphore at which run() method waits for the simulation to be initialised
         * so that the SFML window is not frozen
         */
        boost::interprocess::interprocess_semaphore sfmlWindowSemaphore_;

        bool terminated_ = false;
        bool toTerminate_ = false;
        bool pausedSimulation_ = false;

        Program(const Program &) = delete;
        Program(Program &&) = delete;
        Program &operator=(Program &&) = delete;
        Program &operator=(const Program &) = delete;
        void terminate();

        void sendStatistics(const SimulationData &data);

    public:
        Program();
        ~Program() = default;

        /**
         * Main method of the program, incl. SFML window handler
         */
        void run();

        /**
         * Calls the string as Javascript on the webview
         */
        void callJS(const std::string &javascript);

        /**
         * Prepares program::run() for terminate
         */
        void callTerminate();

        /**
         * Runs statistics thread
         * 
         * \note
         * This could not be easily located elswhere, as webview library characteristics only allow for
         * inclusion of the header only in a single compilation unit.
         */
        void runStatistics();
    };
}; // namespace zpr