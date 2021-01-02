/////////////////////////////////////////////////////////
///                                                   ///
///     Author: Bartlomiej Janowski                   ///
///     This is the main class of the application     ///
///     all important data and objects are held       ///
///     behind the pointers inside this class.        ///
///                                                   ///
/////////////////////////////////////////////////////////
#pragma once

//C++ Standard Library stuff.
#include <memory>
#include <string>
#include <thread>

namespace webview
{
    class webview;
};

namespace sf
{
    class RenderWindow;
}

class Simulation;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const std::string WINDOW_NAME = "CreatureSim";

class Program
{
    std::shared_ptr<sf::RenderWindow> programWindowPtr_;
    std::shared_ptr<webview::webview> webviewPtr_;
    std::shared_ptr<Simulation> simulationPtr_;
    std::thread webviewThread_;

public:
    Program();
    virtual ~Program();
    void run();
};