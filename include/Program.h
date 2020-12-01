#pragma once

//C++ Standard Library stuff.
#include <memory>
#include <string>
#include <thread>

namespace webview {
    class webview;
};


namespace sf {
    class RenderWindow;
}

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const std::string WINDOW_NAME = "CreatureSim";

class Program
{
    std::unique_ptr<sf::RenderWindow> programWindowPtr_;
    std::unique_ptr<webview::webview> webviewPtr_;

public:
    Program();
    virtual ~Program();
    void run();
};