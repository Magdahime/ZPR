#include <iostream>
#include <thread>
#include <cmath>

//SFML stuff.
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Webview
#include "webview.h"
#include "WebviewHtml.h"

#include "Program.h"
#include "Simulation.h"
#include "Map.h"
#include "Perlin.h"

#include "CreatureFactory.h"

using namespace std;

Program::~Program(){};

Program::Program()
{
    programWindowPtr_ = make_shared<sf::RenderWindow>(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME);
    programWindowPtr_->setVerticalSyncEnabled(true);
    #ifdef LINUX_WV
    webviewThread_ = thread([&] {
    #endif //LINUX_WV
    webviewPtr_ = make_shared<webview::webview>(true, nullptr);
    webviewPtr_->set_title("WebView Interface");
    webviewPtr_->set_size(480, 320, WEBVIEW_HINT_NONE);
    webviewPtr_->set_size(180, 120, WEBVIEW_HINT_MIN);
    #ifdef LINUX_WV
    webviewPtr_->run();
    #endif //LINUX_WV
    #ifdef LINUX_WV
    });
    #endif //LINUX_WV
    simulationPtr_ = make_shared<Simulation>();
}

void Program::run()
{
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    sf::View simView;

    shared_ptr<Map> mapPtr;

    bool submitted = false;

    thread simulationThread;
    #ifdef LINUX_WV
    webviewPtr_->dispatch([&] { 
    #endif //LINUX_WV
    webviewPtr_->bind(
        "setMapSize",
        [&](std::string s) -> std::string {
            auto windowWidth = std::stoi(webview::json_parse(s, "", 0));
            auto windowHeight = std::stoi(webview::json_parse(s, "", 1));
            mapPtr = make_shared<Map>(windowWidth, windowHeight);
            auto perlin = Perlin(windowWidth, windowHeight);
            auto pixels = mapPtr->generateMapFromPerlin(perlin);
            image.create(windowWidth, windowHeight, pixels);
            texture.create(windowWidth, windowHeight);
            texture.update(image);
            sprite.setTexture(texture);
            simView.setCenter(windowWidth/2.f, windowHeight/2.f);
            simView.setSize(windowWidth, windowHeight);
            simView.zoom(.06125f);
            simView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
            programWindowPtr_->setView(simView);
            submitted = true;
            simulationPtr_->setMap(mapPtr);
            simulationThread = thread([this] { simulationPtr_->run(); });
            return "OK";
        }
    );
    #ifdef LINUX_WV
    });
    #endif //LINUX_WV

    #ifdef LINUX_WV
    webviewPtr_->dispatch([&] { 
    #endif //LINUX_WV
    // webviewPtr_->init(HtmlFiles::WEBVIEW_JS_INIT);
    webviewPtr_->navigate("http://127.0.0.1:2137/");
    // webviewPtr_->navigate(HtmlFiles::WEBVIEW_HTML_STR);
    #ifdef LINUX_WV
    });
    #endif //LINUX_WV

    auto webviewThread = thread([this] { webviewPtr_->run(); });
    unsigned int frameCounter = 0;
    time_t now = time(0);
    time_t newnow;
    bool moving = false;
    sf::Vector2f oldPos;

    while (programWindowPtr_->isOpen())
    {
        sf::Event event;
        while (programWindowPtr_->pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                programWindowPtr_->close();
                break;
            case sf::Event::MouseWheelMoved:
                {
                    simView = programWindowPtr_->getView();
                    simView.zoom(pow(2.0f, event.mouseWheel.delta*0.5));
                    sf::Vector2i pixelPos = sf::Mouse::getPosition(*programWindowPtr_);
                    sf::Vector2f worldPos = programWindowPtr_->mapPixelToCoords(pixelPos);
                    sf::Vector2f currCenter = simView.getCenter();
                    worldPos = (worldPos + currCenter) * 0.5f;
                    simView.setCenter(worldPos);
                    programWindowPtr_->setView(simView);
                    std::cout << simView.getSize().x << "\t" << simView.getSize().y << "\n";
                    break;
                }
            case sf::Event::MouseButtonPressed:
                    if (event.mouseButton.button == 0) {
                        moving = true;
                        oldPos = programWindowPtr_->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    }
                    break;
            case  sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == 0) {
                    moving = false;
                }
                break;
            case sf::Event::MouseMoved:
                {
                    if (!moving)
                        break;
                    sf::Vector2f newPos = programWindowPtr_->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                    sf::Vector2f deltaPos = oldPos - newPos;
                    simView = programWindowPtr_->getView();
                    simView.setCenter(simView.getCenter() + deltaPos);
                    programWindowPtr_->setView(simView);
                    oldPos = programWindowPtr_->mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y));
                    break;
                }
            default:
                break;
            }
            
        }
        ++frameCounter;
        newnow = time(0);
        if(newnow != now){
            std::cout<<"FPS:\t"<<frameCounter<<"\n";
            frameCounter=0;
        }
        now = newnow;
        #ifdef LINUX_WV
        webviewPtr_->dispatch([&] { 
        #endif //LINUX_WV
            webviewPtr_->eval("frameNum(" + to_string(frameCounter) + ");");
        #ifdef LINUX_WV
        });
        #endif //LINUX_WV
        
        simulationPtr_->postVideo();

        programWindowPtr_->clear();
        programWindowPtr_->draw(sprite);
        simView = programWindowPtr_->getView();

        // simulationPtr_->printClipped_PROTO(programWindowPtr_, simView);
        simulationPtr_->printClipped(programWindowPtr_, simView);

        // simulationPtr_->printAll_PROTO(programWindowPtr_.get());

        programWindowPtr_->display();
    }
}