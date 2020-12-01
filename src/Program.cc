#include <iostream>
#include <thread>

//SFML stuff.
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

//GUI stuff
#include <TGUI/TGUI.hpp>

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
    mainGuiPtr_ = make_unique<tgui::GuiSFML>();
    webviewPtr_ = make_unique<webview::webview>(true, nullptr);
    mainGuiPtr_->setTarget(*programWindowPtr_);
    webviewPtr_->set_title("WebView Interface");
    webviewPtr_->set_size(480, 320, WEBVIEW_HINT_NONE);
    webviewPtr_->set_size(180, 120, WEBVIEW_HINT_MIN);
    webviewPtr_->navigate(WEBVIEW_HTML_STR);
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

    mainGuiPtr_->loadWidgetsFromFile("resources/gui/gui1.txt");
    auto slider = mainGuiPtr_->get<tgui::Slider>("PopSizeSlider");
    auto popCounter = mainGuiPtr_->get<tgui::Label>("PopSizeCounter");
    auto submitButton = mainGuiPtr_->get<tgui::Button>("ConfirmButt");
    auto configPanel = mainGuiPtr_->get<tgui::Panel>("ConfigPanel");

    tgui::Button::Ptr messageSendBtn;
    tgui::TextArea::Ptr messageContent;

    submitButton->onPress([&] {
        auto perlinSize = (int)slider->getValue();
        mapPtr = make_unique<Map>(perlinSize, perlinSize);
        perlinPtr = make_unique<Perlin>(perlinSize, perlinSize);
        pixels = mapPtr->generateMapFromPerlin(*perlinPtr);
        image.create(perlinSize, perlinSize, pixels);
        texture.create(perlinSize, perlinSize);
        texture.update(image);
        sprite.setTexture(texture);
        submitted = true;
        mainGuiPtr_->removeAllWidgets();
        mainGuiPtr_->loadWidgetsFromFile("resources/gui/gui2.txt");
        messageSendBtn = mainGuiPtr_->get<tgui::Button>("sendMsgBtn");
        messageContent = mainGuiPtr_->get<tgui::TextArea>("MsgArea1");
        messageSendBtn->onPress([&] {
            cout<<messageContent->getText().toAnsiString();
            webviewPtr_->eval("writeToBody(\"" + messageContent->getText().toAnsiString() +"\")");
            });
    });
    while (programWindowPtr_->isOpen())
    {
        sf::Event event;
        while (programWindowPtr_->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                programWindowPtr_->close();
        }

        mainGuiPtr_->handleEvent(event);
        programWindowPtr_->clear();
        programWindowPtr_->draw(sprite);
        if (!submitted)
        {
            popCounter->setText(std::to_string((int)slider->getValue()));
        }
        mainGuiPtr_->draw();
        programWindowPtr_->display();
    }
}