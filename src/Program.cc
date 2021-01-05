#include <iostream>
#include <cmath>

// Webview
#include "webview.h"

#include "Program.h"
#include "Simulation.h"
#include "Map.h"
#include "Perlin.h"
#include "Flags.h"

#include "CreatureFactory.h"

using namespace std;

Program::~Program(){};

Program::Program()
{
    webviewPtr_ = std::make_shared<webview::webview>(zpr_dev_flags::WEBVIEW_DEBUG, nullptr);
    programWindowPtr_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(zpr_windows::SF_X, zpr_windows::SF_Y), zpr_windows::SF_NAME),
    programWindowPtr_->setVerticalSyncEnabled(true);
    webviewPtr_->set_title(zpr_windows::WV_NAME);
    webviewPtr_->set_size(zpr_windows::WV_X, zpr_windows::WV_Y, WEBVIEW_HINT_FIXED);
    webviewPtr_->navigate(zpr_paths::HTTP_PATH);
    webviewThread_ = thread([&] {
        webviewPtr_->run();
    });
}

void Program::run()
{
    shared_ptr<Map> mapPtr;

    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;

    sf::View simView;

    bool submitted = false;

    thread simulationThread;
#ifdef LINUX_WV
    webviewPtr_->dispatch([&] {
#endif //LINUX_WV
        webviewPtr_->bind(
            "setMapSize",
            [&](std::string s) -> std::string {
                if (submitted)
                    return "ALREADY SUBMITTED";
                auto windowWidth = std::stoi(webview::json_parse(s, "", 0));
                auto windowHeight = std::stoi(webview::json_parse(s, "", 1));
                mapPtr = make_shared<Map>(windowWidth, windowHeight);
                auto perlin = Perlin(windowWidth, windowHeight);
                auto pixels = mapPtr->generateMapFromPerlin(perlin);
                image.create(windowWidth, windowHeight, pixels);
                texture.create(windowWidth, windowHeight);
                texture.update(image);
                sprite.setTexture(texture);
                simView.setCenter(windowWidth / 2.f, windowHeight / 2.f);
                simView.setSize(windowWidth, windowHeight);
                simView.zoom(.06125f); //alert MAGIC
                simView.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
                programWindowPtr_->setView(simView);
                submitted = true;
                simulation_.setMap(mapPtr);
                return "OK";
            });

            webviewPtr_->bind(
                "setSimulationParameters",
                [&](std::string s) -> std::string{
                    SimulationParameters newParams;
                    newParams.creaturesNum_ = std::stoi(webview::json_parse(s, "", 0));
                    newParams.energyThreshhold_ = std::stof(webview::json_parse(s, "", 1));
                    newParams.minWeight_ = std::stof(webview::json_parse(s, "", 2));
                    newParams.birthWeightThreshhold_= std::stof(webview::json_parse(s, "", 3));
                    newParams.energyBirth_= std::stof(webview::json_parse(s, "", 4));
                    newParams.energyBirthFailed_= std::stof(webview::json_parse(s, "", 5));
                    newParams. weightBirth_= std::stof(webview::json_parse(s, "", 6));
                    newParams.birthAgeThreshhold_= std::stof(webview::json_parse(s, "", 7));
                    newParams.anglePerFrame_= std::stof(webview::json_parse(s, "", 8));
                    newParams.accelerationMultiplier_= std::stof(webview::json_parse(s, "", 9));
                    newParams.maxSpeed_= std::stof(webview::json_parse(s, "", 10));
                    simulation_.setSimulationParameters(newParams);
                    simulationThread = thread([this] { simulation_.run();});
                    return "OK";
                });
#ifdef LINUX_WV
    });
#endif //LINUX_WV
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
                webviewPtr_->terminate();
                programWindowPtr_->close();
                break;
            case sf::Event::MouseWheelMoved:
            {
                simView = programWindowPtr_->getView();
                simView.zoom(pow(2.0f, event.mouseWheel.delta * 0.5));
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
                if (event.mouseButton.button == 0)
                {
                    moving = true;
                    oldPos = programWindowPtr_->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                }
                break;
            case sf::Event::MouseButtonReleased:
                if (event.mouseButton.button == 0)
                {
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
        if (newnow != now)
        {
            std::cout << "FPS:\t" << frameCounter << "\n";
            frameCounter = 0;
        }
        now = newnow;
#ifdef LINUX_WV
        webviewPtr_->dispatch([&] {
#endif //LINUX_WV
            webviewPtr_->eval("frameNum(" + to_string(frameCounter) + ");");
#ifdef LINUX_WV
        });
#endif //LINUX_WV

        simulation_.postVideo();

        programWindowPtr_->clear();
        programWindowPtr_->draw(sprite);
        simView = programWindowPtr_->getView();
        simulation_.printClipped(programWindowPtr_, simView);
        programWindowPtr_->display();
    }
}