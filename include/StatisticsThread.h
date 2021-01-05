/////////////////////////////////////////////////////////
///     Author: Bartlomiej Janowski                   ///
///     Statistics gathering thread                   ///
///                                                   ///
/////////////////////////////////////////////////////////
#pragma once

#include <thread>
#include <memory>

class Simulation;

class StatisticsThread
{
    std::thread thread_;
    std::shared_ptr<Simulation> simulationPtr_;
    bool terminate_ = false;

    void processData();

public:
    StatisticsThread(std::shared_ptr<Simulation> simulationPtr);
    StatisticsThread();
    void run();
    void terminate() { terminate_ = true; };
};