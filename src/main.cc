//File: main.cpp

//Program: SDL_HelloWorld

#include <iostream>
#include "map.h"
using std::cout;
using std::endl;

//SDL stuff.
#include <SFML/Graphics.hpp>

int main (int argc, char* argv[]) {
    int width = atoi(argv[1]);
    int height = atoi(argv[2]);
    sf::RenderWindow window(sf::VideoMode(width, height), "CreatureSim");
    sf::Image image;
    sf::Texture texture;
    sf::Sprite sprite;
    Map map(width, height);
    unsigned char * pixels = map.generateR();
    image.create(width,height,pixels);
    texture.create(width,height);
    texture.update(image);
    sprite.setTexture(texture);
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(sprite);
        window.display();
    }

    return 0;
};
