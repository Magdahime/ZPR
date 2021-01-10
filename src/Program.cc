
#pragma warning(push, 0)

#include <iostream>
#include <cmath>

#include "webview.h"

#pragma warning(pop)

#include "Program.h"
#include "Simulation.h"
#include "PyWebserver.h"
#include "Map.h"
#include "Perlin.h"
#include "Flags.h"
#include "JsonParser.h"
#include "CreatureFactory.h"

using namespace std;

Program::~Program(){};

Program::Program() : webviewSemaphore_(0)
{
    simulationPtr_ = std::make_shared<Simulation>();
    webserverPtr_ = std::make_shared<PyWebserver>();
    webserverPtr_->run();
    programWindowPtr_ = std::make_shared<sf::RenderWindow>(sf::VideoMode(zpr_windows::SF_X, zpr_windows::SF_Y), zpr_windows::SF_NAME),
    programWindowPtr_->setVerticalSyncEnabled(true);

    webviewThread_ = std::thread([this]() {
        webviewPtr_ = std::make_shared<webview::webview>(zpr_dev_flags::WEBVIEW_DEBUG, nullptr);
        webviewSemaphore_.post();
        webviewPtr_->set_title(zpr_windows::WV_NAME);
        webviewPtr_->set_size(zpr_windows::WV_X, zpr_windows::WV_Y, WEBVIEW_HINT_FIXED);
        webviewPtr_->navigate(zpr_paths::HTTP_PATH);
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

    bool submittedMap = false;
    bool submittedParams = false;

    webviewSemaphore_.wait();

    statisticsThread_ = std::thread(&Program::runStatistics, this);

    webviewPtr_->dispatch([&] {
        webviewPtr_->bind(
            "setMapSize",
            [&](std::string s) -> std::string {
                if (submittedMap)
                    return "ALREADY SUBMITTED";
                submittedMap = true;
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
                simulationPtr_->setMap(mapPtr);
                return "OK";
            });

        webviewPtr_->bind(
            "setSimulationParameters",
            [&](std::string s) -> std::string {
                if (submittedParams)
                    return "ALREADY SUBMITTED";
                submittedParams = true;
                SimulationParameters newParams;
                newParams.creaturesNum_ = std::stoi(webview::json_parse(s, "", 0));
                newParams.energyThreshhold_ = std::stof(webview::json_parse(s, "", 1));
                newParams.minWeight_ = std::stof(webview::json_parse(s, "", 2));
                newParams.weightGained_ = std::stof(webview::json_parse(s, "", 3));
                newParams.weightLost_ = std::stof(webview::json_parse(s, "", 4));
                newParams.birthWeightThreshhold_ = std::stof(webview::json_parse(s, "", 5));
                newParams.energyBirth_ = std::stof(webview::json_parse(s, "", 6));
                newParams.energyBirthFailed_ = std::stof(webview::json_parse(s, "", 7));
                newParams.weightBirth_ = std::stof(webview::json_parse(s, "", 8));
                newParams.birthAgeThreshhold_ = std::stof(webview::json_parse(s, "", 9));
                newParams.anglePerFrame_ = std::stof(webview::json_parse(s, "", 10));
                newParams.accelerationMultiplier_ = std::stof(webview::json_parse(s, "", 11));
                newParams.maxSpeed_ = std::stof(webview::json_parse(s, "", 12));
                simulationPtr_->setSimulationParameters(newParams);
                submittedParams = true;
                simulationThread_ = thread([this] { simulationPtr_->run(); });
                return "OK";
            });

        webviewPtr_->bind(
            "putCreature",
            [&](std::string s) -> std::string {
                if (submittedParams)
                {
                    std::string filename = "userDefined";
                    std::string creatureData = webview::json_parse(s, "", 0);
                    std::string creatureNum = webview::json_parse(s, "", 1);
                    if (!std::filesystem::exists(JsonParser::SAVE_PATH + webview::json_parse(creatureData, "type", 0)))
                    {
                        std::string path = JsonParser::saveJsonToFile(filename, creatureData);
                        CreatureFactory::getInstance().registerCreature(path);
                        simulationPtr_->putCreature(webview::json_parse(creatureData, "type", 0), std::stoi(creatureNum));
                    }
                    return "OK";
                }
                else
                {
                    webviewPtr_->eval("NotSubmitted()");
                    return "NOT OK";
                }
            });
        webviewPtr_->bind(
            "getListOfCreatures",
            [&](std::string s) -> std::string {
                webviewPtr_->eval("addCreaturesToDropdown(" + CreatureFactory::getInstance().parseKeys() + ");");
                return "OK";
            });
        webviewPtr_->bind("getDataAboutCreature",
                          [&](std::string s) -> std::string {
                              std::cout << webview::json_parse(s, "", 0) << std::endl;
                              webviewPtr_->eval("receiveData(" + CreatureFactory::getInstance().getParsedValues(webview::json_parse(s, "", 0)) + ")");
                              return "OK";
                          });
    });
    unsigned int frameCounter = 0;
    time_t now = time(0);
    time_t newnow;
    bool moving = false;
    bool webviewClosed = false;
    sf::Vector2f oldPos;

    while (programWindowPtr_->isOpen())
    {
        try
        {
            webviewPtr_->dispatch([this, frameCounter] {
                webviewPtr_->eval("frameNum(" + to_string(frameCounter) + ");");
            });
        }
        catch (...)
        {
            webviewClosed = true;
            break;
        }
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
                else if (event.mouseButton.button == 1)
                {
                    auto pos = programWindowPtr_->mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    std::cout << "CIPA";
                    simulationPtr_->selectClosestCreature(pos.x, pos.y);
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
        if (simulationPtr_->isSelected())
        {
            simView = programWindowPtr_->getView();
            simView.setCenter(sf::Vector2f(simulationPtr_->getSelectedX(), simulationPtr_->getSelectedY()));
            programWindowPtr_->setView(simView);
        }
        ++frameCounter;
        newnow = time(0);
        if (newnow != now)
        {
            std::cout << "FPS:\t" << frameCounter << "\n";
            frameCounter = 0;
        }
        now = newnow;

        simulationPtr_->postVideo();

        programWindowPtr_->clear();
        programWindowPtr_->draw(sprite);
        simView = programWindowPtr_->getView();
        simulationPtr_->printClipped(programWindowPtr_, simView);
        programWindowPtr_->display();
    }
    terminateStatistics();
    simulationPtr_->terminate();
    programWindowPtr_->close();
    webviewPtr_->dispatch([this] {
        webviewPtr_->terminate();
    });
    webviewPtr_->terminate();

    webviewThread_.join();
    std::cout << "\nWebview done\n";

    simulationThread_.join();
    std::cout << "\nSim done\n";

    webserverPtr_->terminate();
    std::cout << "\nServer terminate done\n";
    webserverPtr_->join();
    std::cout << "\nServer done\n";
}

void Program::callJS(const std::string &javascript)
{
    // std::cout << "\n"
    //           << javascript << "\n";
    webviewPtr_->dispatch([this, javascript] {
        webviewPtr_->eval(javascript);
    });
}

void Program::runStatistics()
{
    auto end = std::chrono::steady_clock::now() + std::chrono::milliseconds(zpr_consts::statistics_sleep_millis);
    auto increment = std::chrono::milliseconds(zpr_consts::statistics_sleep_millis);
    while (!terminate_)
    {
        std::cout << "Works!";
        SimulationData data;
        data.secondNum = simulationPtr_->getSimulationSecond();
        data.populationSize = simulationPtr_->getPopulationSize();
        data.totalWeight = simulationPtr_->getTotalWeight();
        data.avgAge = simulationPtr_->getAvgAge();
        sendStatistics(data);
        if (simulationPtr_->isSelected())
        {
            std::string params = simulationPtr_->getSelectedParametersAsJSON();
            std::string neurons = simulationPtr_->getSelectedNeuronsAsJSON();
            callJS(
                std::string("updateDataAboutCreature(") + params + std::string(");"));
            callJS(
                std::string("updateDataAboutNetwork(") + neurons + std::string(");"));
        }
        auto sleeptime = std::chrono::duration_cast<std::chrono::milliseconds>(end - std::chrono::steady_clock::now());
        end += increment;
        std::this_thread::sleep_for(sleeptime);
    }
}

void Program::sendStatistics(const SimulationData &data)
{
    callJS(std::string("newDataPopulationNum(") + std::to_string(data.secondNum) + std::string(", ") + std::to_string(data.populationSize) + std::string(");"));
    callJS(std::string("newDataWeightNum(") + std::to_string(data.secondNum) + std::string(", ") + std::to_string(data.totalWeight) + std::string(");"));
    callJS(std::string("newDataAgeNum(") + std::to_string(data.secondNum) + std::string(", ") + std::to_string(data.avgAge) + std::string(");"));
}