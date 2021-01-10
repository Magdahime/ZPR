/////////////////////////////////////////////////////////
///                                                   ///
///     Author: Bartlomiej Janowski                   ///
///     This is the main class of the application     ///
///     all important data and objects are held       ///
///     behind the pointers inside this class.        ///
///                                                   ///
/////////////////////////////////////////////////////////
#pragma once

#pragma warning(push, 0)

//C++ Standard Library stuff.
#include <memory>
#include <string>
#include <thread>

//SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#pragma warning(pop)

// ZPR
#include "Simulation.h"

namespace webview
{
    class webview;
}

class PyWebserver;

/**
 * \author Bartłomiej Janowski
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
        unsigned int secondNum;
        unsigned int populationSize;
        float totalWeight;
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

    boost::interprocess::interprocess_semaphore webviewSemaphore_;

    bool terminate_ = false;

    Program(const Program &) = delete;
    Program(Program &&) = delete;
    Program &operator=(Program &&) = delete;
    Program &operator=(const Program &) = delete;

public:
    Program();
    virtual ~Program();
    void run();

    void callJS(const std::string &javascript);

    void runStatistics();
    void sendStatistics(const SimulationData &data);
    void terminateStatistics() { terminate_ = true; };
};
