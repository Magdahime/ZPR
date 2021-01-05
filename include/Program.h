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

//SFML
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// ZPR
#include "Simulation.h"


namespace webview {
    class webview;
}

class Program
{
    std::shared_ptr<sf::RenderWindow> programWindowPtr_;
    std::shared_ptr<webview::webview> webviewPtr_;
    Simulation simulation_;
    std::thread webviewThread_;

public:
    Program();
    virtual ~Program();
    void run();
};