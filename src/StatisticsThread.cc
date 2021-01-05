#include <chrono>

#include "StatisticsThread.h"
#include "Simulation.h"
#include "Flags.h"



StatisticsThread::StatisticsThread(std::shared_ptr<Simulation> simulationPtr) : simulationPtr_(simulationPtr){};
StatisticsThread::StatisticsThread() : simulationPtr_(nullptr){};


void StatisticsThread::run()
{
    thread_ = std::thread(&StatisticsThread::processData, this);
};

void StatisticsThread::processData()
{
    auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(zpr_consts::statistics_sleep_millis);
    auto increment = std::chrono::milliseconds(zpr_consts::statistics_sleep_millis);
    while (!terminate_)
    {
        std::cout<<"Works!";
        auto sleeptime = std::chrono::duration_cast<std::chrono::milliseconds>(end - std::chrono::steady_clock::now());
        end += increment;
        std::this_thread::sleep_for(sleeptime);
    }
}