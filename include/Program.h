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
    // StatisticsThread statThread_;
    SimulationData statistics_;

    boost::interprocess::interprocess_semaphore webviewSemaphore_;

    bool terminate_ = false;

public:
    Program();
    virtual ~Program();
    void run();

    void callJS(const std::string &javascript);

    void runStatistics();
    void sendStatistics(const SimulationData &data);
    void terminateStatistics() { terminate_ = true; };
};
