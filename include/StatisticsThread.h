/////////////////////////////////////////////////////////
///     Author: Bartlomiej Janowski                   ///
///     Statistics gathering thread                   ///
///                                                   ///
/////////////////////////////////////////////////////////
#pragma once

#include <thread>
#include <memory>

namespace webview{
    class webview;
}

class Simulation;

struct SimulationData
{
    unsigned int frameNum;
    unsigned int populationSize;
    float totalWeight;
    float maxAge;
};

class StatisticsThread
{
    std::thread thread_;
    std::shared_ptr<Simulation> simulationPtr_;
    std::shared_ptr<webview::webview> webviewPtr_;
    bool terminate_ = false;

    void processData();

public:
    StatisticsThread(std::shared_ptr<Simulation> simulationPtr);
    StatisticsThread();

    void setWebviewPtr(std::shared_ptr<webview::webview> webviewPtr){
        webviewPtr_ = webviewPtr;
    }

    void sendData(const SimulationData& data);

    void run();
    void terminate() { terminate_ = true; };
};