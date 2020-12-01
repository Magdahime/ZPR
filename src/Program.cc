#include <iostream>
#include <thread>

//SFML stuff.
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// Webview
#include "webview.h"
#include "WebviewHtml.h"

#include "Program.h"

#include "map.h"
#include "perlin.h"

using namespace std;

Program::~Program(){};

Program::Program()
{
    programWindowPtr_ = make_unique<sf::RenderWindow>(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_NAME);
    programWindowPtr_->setVerticalSyncEnabled(true);
    webviewPtr_ = make_unique<webview::webview>(true, nullptr);
    webviewPtr_->set_title("WebView Interface");
    webviewPtr_->set_size(480, 320, WEBVIEW_HINT_NONE);
    webviewPtr_->set_size(180, 120, WEBVIEW_HINT_MIN);
}

void Program::run()
{
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    

    unsigned char *pixels;
    unique_ptr<Map> mapPtr;
    unique_ptr<Perlin> perlinPtr;

    bool submitted = false;

    webviewPtr_->bind("setMapSize",[&](std::string s) -> std::string {
        auto windowWidth = std::stoi(webview::json_parse(s, "", 0));
        auto windowHeight = std::stoi(webview::json_parse(s, "", 1));
        mapPtr = make_unique<Map>(windowWidth, windowHeight);
        perlinPtr = make_unique<Perlin>(windowWidth, windowHeight);
        pixels = mapPtr->generateMapFromPerlin(*perlinPtr);
        image.create(windowWidth, windowHeight, pixels);
        texture.create(windowWidth, windowHeight);
        texture.update(image);
        sprite.setTexture(texture);
        submitted = true;
        return "OK";
    });
    webviewPtr_->navigate(WEBVIEW_HTML_STR);
    auto webviewThread = thread([this] {webviewPtr_->run();});
    unsigned int frameCounter = 0;
    while (programWindowPtr_->isOpen())
    {
        sf::Event event;
        while (programWindowPtr_->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                programWindowPtr_->close();
        }
        programWindowPtr_->clear();
        programWindowPtr_->draw(sprite);
        if (!submitted)
        {
        }
        programWindowPtr_->display();
        ++frameCounter;
        webviewPtr_->eval("frameNum(" + to_string(frameCounter) + ");");
    }
}