#include <iostream>
#include <cmath>
#include <omp.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Simulation.h"
#include "Creature.h"
#include "Neuron.h"
#include "Map.h"


using namespace std;

Simulation::~Simulation(){};

Simulation::Simulation() : dataSemaphore_(1) {};



/*
    This is just a prototype to show that openMP is a viable way of parallelisation.
    It will be replaced with a more sensible solution as soon as possible.
*/
void Simulation::run_PROTO()
{
    data_PROTO_.resize(CREATURE_COUNT_PROTO * 24);
    int xSize = map_->getWidth();
    int ySize = map_->getHeight();
#pragma omp parallel for
    for (int i = 0; i < data_PROTO_.size(); i += 24)
    {
        RandomNumberGenerator<float> rngX(0, xSize);
        RandomNumberGenerator<float> rngY(0, ySize);
        RandomNumberGenerator<float> rngHdg(0.0f, 2 * pi);
        RandomNumberGenerator<float> rngSpeed(0.0f, (xSize > ySize) ? xSize * 0.001f : ySize * 0.001f);
        RandomNumberGenerator<float> rngSize(0.0f, 10.0f);
        RandomNumberGenerator<float> rngWeight(-1.0f, 1.0f);
        int x = rngX.get();
        int y = rngY.get();
        data_PROTO_[i] = x;
        data_PROTO_[i + 1] = y;
        data_PROTO_[i + 2] = rngHdg.get();
        data_PROTO_[i + 3] = rngSpeed.get();
        data_PROTO_[i + 4] = rngSize.get();
        data_PROTO_[i + 5] = map_->getPixel(x, y, 0);
        data_PROTO_[i + 6] = map_->getPixel(x, y, 1);
        data_PROTO_[i + 7] = map_->getPixel(x, y, 2);
        for (int j = 8; j < 24; j++)
        {
            data_PROTO_[i + j] = rngWeight.get();
        }
    }
    cout << "Done RUN_PROTO x: " << xSize << "y: " << ySize << "\n";
    int i = 0;
    time_t now = time(0);
    time_t newnow;
    dataSemaphore_.post();
    while (true)
    {
        dataSemaphore_.wait();
        i++;
        newnow = time(0);
        if(newnow != now){
            std::cout<<"IPS:\t"<<i<<"\n";
            i=0;
        }
        now = newnow;
        iteration_PROTO();
    }
};

void Simulation::iteration_PROTO()
{
    #pragma omp parallel for
    for (int i = 0; i < data_PROTO_.size(); i += 24)
    {
        float speedResult = 0.0f;
        float hdgResult = 0.0f;
        int xSize = map_->getWidth();
        int ySize = map_->getHeight();
        for (int j = 0; j < 8; ++j)
        {
            speedResult += data_PROTO_[i + j] * data_PROTO_[i + j + 8];
            hdgResult += data_PROTO_[i + j] * data_PROTO_[i + j + 16];
        }
        data_PROTO_[i + 3] *= (speedResult < 0.0f) ? 0.8f : 1.2f;
        if (data_PROTO_[i + 3] > ((xSize > ySize) ? xSize * 0.0002f : ySize * 0.0002f))
        {
            data_PROTO_[i + 3] = ((xSize > ySize) ? xSize * 0.0002f : ySize * 0.0002f);
        }
        data_PROTO_[i + 2] += (hdgResult < 1.0f) ? pi / 6.0f : (hdgResult > 1.0f) ? -pi / 6.0f : 0.0f;
        if (data_PROTO_[i + 2] < 0)
        {
            data_PROTO_[i + 2] += 2 * pi;
        }
        if (data_PROTO_[i + 2] > 2 * pi)
        {
            data_PROTO_[i + 2] -= 2 * pi;
        }
        data_PROTO_[i] += sin(data_PROTO_[i + 2]) * data_PROTO_[i + 3];
        data_PROTO_[i + 1] += cos(data_PROTO_[i + 2]) * data_PROTO_[i + 3];
        if (data_PROTO_[i] > map_->getWidth())
        {
            data_PROTO_[i] -= map_->getWidth();
        }
        else if (data_PROTO_[i] < 0)
        {
            data_PROTO_[i] += map_->getWidth();
        }
        if (data_PROTO_[i + 1] > map_->getHeight())
        {
            data_PROTO_[i + 1] -= map_->getHeight();
        }
        else if (data_PROTO_[i + 1] < 0)
        {
            data_PROTO_[i + 1] += map_->getHeight();
        }
        int x = data_PROTO_[i];
        int y = data_PROTO_[i + 1];
        data_PROTO_[i + 5] = map_->getPixel(x, y, 0);
        data_PROTO_[i + 6] = map_->getPixel(x, y, 1);
        data_PROTO_[i + 7] = map_->getPixel(x, y, 2);
    }
    // dataSemaphore_.unlock();
};

void Simulation::printAll_PROTO(sf::RenderWindow *window)
{
    for (int i = 0; i < data_PROTO_.size(); i += 24)
    {
        sf::CircleShape circle;
        circle.setPosition(data_PROTO_[i] - data_PROTO_[i + 4] / 2.0f, data_PROTO_[i + 1] - data_PROTO_[i + 4] / 2.0f);
        circle.setRadius(data_PROTO_[i + 4]);
        circle.setFillColor(sf::Color(data_PROTO_[i + 5] * 0.5f, data_PROTO_[i + 6] * 0.5f, data_PROTO_[i + 7] * 0.5f));
        window->draw(circle);
    }
};

void Simulation::printClipped_PROTO(std::shared_ptr<sf::RenderWindow> window, sf::View view){
    float xMin, xMax, yMin, yMax;
    xMin = view.getCenter().x - view.getSize().x / 2.f;
    xMax = view.getCenter().x + view.getSize().x / 2.f; 
    yMin = view.getCenter().y - view.getSize().y / 2.f;
    yMax = view.getCenter().y + view.getSize().y / 2.f; 
    for (int i = 0; i < data_PROTO_.size(); i += 24)
    {
        float xPos = data_PROTO_[i];
        float yPos = data_PROTO_[i + 1];
        if(xMin <= xPos && xPos <= xMax && yMin <= yPos && yPos <= yMax){
            sf::CircleShape circle;
            circle.setPosition(data_PROTO_[i] - data_PROTO_[i + 4] / ( 20.f * 2.f ), data_PROTO_[i + 1] - data_PROTO_[i + 4] / ( 20.f * 2.f ));
            circle.setRadius(data_PROTO_[i + 4] / 20.f);
            circle.setFillColor(sf::Color(data_PROTO_[i + 5] * .9f, data_PROTO_[i + 6] * .9f, data_PROTO_[i + 7] * .9f));
            window->draw(circle);
        }
    }
};

bool Simulation::tryNewData()
{
    return dataSemaphore_.try_wait();
}

void Simulation::postVideo(){
    dataSemaphore_.post();
}

void Simulation::setMap(shared_ptr<Map> mapPtr)
{
    map_ = mapPtr;
}