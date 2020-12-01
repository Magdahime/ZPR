#pragma once

//C++ Standard Library stuff.
#include <memory>
#include <string>
#include <thread>

namespace webview {
    class webview;
};

namespace tgui {
    class GuiSFML;
};

namespace sf {
    class RenderWindow;
}

class WebsocketServer;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const std::string WINDOW_NAME = "CreatureSim";

class Program
{
    std::unique_ptr<sf::RenderWindow> programWindowPtr_;
    std::unique_ptr<tgui::GuiSFML> mainGuiPtr_;
    std::unique_ptr<webview::webview> webviewPtr_;
    std::unique_ptr<WebsocketServer> websocketServerPtr_;

public:
    Program();
    virtual ~Program();
    void run();
};