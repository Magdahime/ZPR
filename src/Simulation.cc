#include <iostream>
#include <cmath>
#include <omp.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <boost/chrono.hpp>

#include "Simulation.h"
#include "Creature.h"
#include "Neuron.h"
#include "Map.h"

using namespace std;

Simulation::~Simulation(){};

Simulation::Simulation() : dataSemaphore_(1){};

void Simulation::prepare(unsigned int creatureCount)
{
    for (int i = 0; i < creatureCount; i++)
    {
        container_.putCreature();
        if (i % 1000 == 0)
            std::cout << "i:\t" << i << "\n";
    }
}
void Simulation::prepare()
{
    prepare(CREATURE_COUNT_PROTO);
}
void Simulation::run()
{
    prepare();
    int i = 0;
    time_t now = time(0);
    time_t newnow;
    dataSemaphore_.post();
    int c = 0;
    while (true)
    {
        dataSemaphore_.wait();
        i++;
        newnow = time(0);
        if (newnow != now)
        {
            std::cout << "IPS:\t" << i << "\t CCount:\t" << c << "\n";
            i = 0;
        }
        now = newnow;
        c = iteration();
    }
}
int Simulation::iteration()
{
    int counter = 0;
#pragma omp parallel for reduction(+ \
                                   : counter)
    for (int creatureIndex = 0; creatureIndex < container_.getSize(); ++creatureIndex)
    {
        if (container_.isDeleted(creatureIndex))
            continue;
        ++counter;
        container_.populateNeurons(creatureIndex);
        for (unsigned int layerIndex = 0; layerIndex < LAYER_WIDTHS.size(); layerIndex++)
        {
            container_.calculateLayer(creatureIndex, layerIndex);
        }
        updateCreature(creatureIndex);
    }
    return counter;
}

void Simulation::updateCreature(int creatureIndex)
{

    std::vector<float> results = container_.getResult(creatureIndex); // 0 -> steer
                                                                      // 1 -> accelerate
                                                                      // 2 -> eat
                                                                      // 3 -> attack
                                                                      // 4 -> birth
    CreatureParametersSPtr creature = container_.getCreatureParameters(creatureIndex);

    if (fabs(results[0]) > ACTIVATION_THRESHOLD)
    {
        if (results[0] > 0)
        {
            creature->heading_ += 2.f * 2.f * pi / 360.f; //alert MAGIC
            creature->heading_ = fmod(creature->heading_ + 2.f * pi, 2.f * pi);
        }
        else
        {
            creature->heading_ -= 2.f * 2.f * pi / 360.f;
            creature->heading_ = fmod(creature->heading_ + 2.f * pi, 2.f * pi);
        }
    }
    if (fabs(results[1]) > ACTIVATION_THRESHOLD) // ACCELERATE
    {
        if (results[1] > 0)
        {
            creature->speed_ *= 1.2f;                         //alert MAGIC
            creature->speed_ = fmin(creature->speed_, 0.05f); //alert MAGIC
        }
        else
        {
            creature->speed_ /= 1.2f; //alert MAGIC
        }
    }
    if (results[2] > ACTIVATION_THRESHOLD) // EAT
    {
        // if (results[2] > 0)
        // {
        float currH = map_->getPixelH(creature->positionX_, creature->positionY_);
        float diffH = fabs(creature->hue_ - currH);
        if (diffH > 180.f)
            diffH = fabs(diffH - 360.f);
        float dist = -((diffH * diffH + 30.f * 30.f) / 180.f - 50.f); //alert MAGIC
        creature->energy_ += dist;
        // }
    }
    bool birth = false;
    if (results[4] > ACTIVATION_THRESHOLD)
    {
        RandomNumberGenerator<float> rng;
        if (creature->weight_ + rng.get(0, BIRTH_WEIGHT_THRESHOLD) > BIRTH_WEIGHT_THRESHOLD)
        {
            birth = true;
            creature->energy_ -= ENERGY_BIRTH;
            creature->weight_ -= WEIGHT_BIRTH;
        }
        else
        {
            creature->energy_ -= ENERGY_BIRTH_FAILED;
        }
    }
    creature->energy_ -= (1000.f + creature->age_) / 20.f / 200.f; //alert MAGIC // idle energy consumption
    creature->speedMultiplier_ = fabs(creature->speed_);
    float movement = creature->speed_ * creature->speedMultiplier_;
    if (creature->energy_ > ENERGY_THRESHOLD)
    {
        creature->weight_ += (creature->energy_ - ENERGY_THRESHOLD);
        creature->energy_ = ENERGY_THRESHOLD;
    }
    else if (creature->energy_ < 0)
    {
        // std::cout << "\nPRZED: " << creatureIndex << " " << creature->weight_;
        creature->weight_ -= fabs(creature->energy_);
        // std::cout << "\nPO: " << creatureIndex << " " << creature->weight_;
        creature->energy_ = 0;
    }

    creature->positionX_ += sin(creature->heading_) * movement;
    (creature->positionX_ > map_->getWidth()) ? creature->positionX_ -= map_->getWidth() : ((creature->positionX_ < 0) ? creature->positionX_ += map_->getWidth() : 0);
    creature->positionY_ += cos(creature->heading_) * movement;
    (creature->positionY_ > map_->getHeight()) ? creature->positionY_ -= map_->getHeight() : ((creature->positionY_ < 0) ? creature->positionY_ += map_->getHeight() : 0);
    creature->bottomAntennaH_ = map_->getPixelH(creature->positionX_, creature->positionY_);
    {
        float xPos = creature->positionX_ + creature->speedMultiplier_ * sin(creature->heading_ - 45.f / 180.f * pi);
        float yPos = creature->positionY_ + creature->speedMultiplier_ * cos(creature->heading_ - 45.f / 180.f * pi);
        creature->rightAntennaH_ = map_->getPixelH(xPos, yPos);
    }
    {
        float xPos = creature->positionX_ + creature->speedMultiplier_ * sin(creature->heading_ + 45.f / 180.f * pi);
        float yPos = creature->positionY_ + creature->speedMultiplier_ * cos(creature->heading_ + 45.f / 180.f * pi);
        creature->leftAntennaH_ = map_->getPixelH(xPos, yPos);
    }
    {
        float xPos = creature->positionX_ - creature->speedMultiplier_ * sin(creature->heading_);
        float yPos = creature->positionY_ - creature->speedMultiplier_ * cos(creature->heading_);
        creature->rearAntennaH_ = map_->getPixelH(xPos, yPos);
    }
    creature->age_ += 1.f / 60.f; //alert MAGIC

    if (birth)
    {
        auto childParams = CreatureFactory::getInstance().createChild(creature);
        auto neurons = container_.getNeurons(creatureIndex);
        auto childNeurons = NeuronFactory::getInstance().createChild(neurons);
        // container_.putCreature(childParams, childNeurons);
    }
    if (creature->weight_ < 10.f)
    { //alert MAGIC
        container_.deleteCreature(creatureIndex);
    }

    container_.updateCreatureParameters(creatureIndex, creature);
}

/*
    This is just a prototype to show that openMP is a viable way of parallelisation.
    It will be replaced with a more sensible solution as soon as possible.
*/
void Simulation::run_PROTO()
{
    auto t1 = boost::chrono::high_resolution_clock::now();
    prepare();
    auto t2 = boost::chrono::high_resolution_clock::now();
    std::cout << "DURATION FOR " << CREATURE_COUNT_PROTO << " CREATURES:\t" << (boost::chrono::duration_cast<boost::chrono::milliseconds>(t2 - t1)) << "\n";
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
        if (newnow != now)
        {
            std::cout << "IPS:\t" << i << "\n";
            i = 0;
        }
        now = newnow;
        iteration_PROTO();

        t1 = boost::chrono::high_resolution_clock::now();
        iteration();
        t2 = boost::chrono::high_resolution_clock::now();
        if (i == 0)
            std::cout << "ITERATION DURATION FOR " << CREATURE_COUNT_PROTO << " CREATURES:\t" << (boost::chrono::duration_cast<boost::chrono::milliseconds>(t2 - t1)) << "\n";
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

void Simulation::printAll(std::shared_ptr<sf::RenderWindow> window)
{
    for (int creatureIndex = 0; creatureIndex < container_.getSize(); ++creatureIndex)
    {
        CreatureParametersSPtr creature = container_.getCreatureParameters(creatureIndex);
        sf::CircleShape circle;
        circle.setPosition(creature->positionX_ - creature->weight_ / 2.0f, creature->positionY_ - creature->weight_ / 2.0f);
        circle.setRadius(creature->weight_ / 2.0f);
        Map::HSVvals hsv(creature->hue_, 1, 1);
        auto rgb = map_->convert2RGB(hsv);
        // if (creatureIndex == 0)
        // {
        //     std::cout << "Index x y w r g b a s\t" << creatureIndex << "\t" << creature->positionX_ << "\t" << creature->positionY_ << "\t" << creature->weight_ << "\t" << rgb.r_ << "\t" << rgb.g_ << "\t" << rgb.b_ << "\tAGE: " << creature->age_ << "\t" << creature->speed_ << "\n";
        // }
        circle.setFillColor(sf::Color(rgb.r_, rgb.g_, rgb.b_));
        window->draw(circle);
    }
}

void Simulation::printClipped(std::shared_ptr<sf::RenderWindow> window, sf::View view)
{
    float xMin, xMax, yMin, yMax;
    xMin = view.getCenter().x - view.getSize().x / 2.f;
    xMax = view.getCenter().x + view.getSize().x / 2.f;
    yMin = view.getCenter().y - view.getSize().y / 2.f;
    yMax = view.getCenter().y + view.getSize().y / 2.f;
    for (int creatureIndex = 0; creatureIndex < container_.getSize(); ++creatureIndex)
    {
        if (container_.isDeleted(creatureIndex))
            continue;
        float xPos = container_.getCreatureX(creatureIndex);
        float yPos = container_.getCreatureY(creatureIndex);
        if (xMin <= xPos && xPos <= xMax && yMin <= yPos && yPos <= yMax)
        {
            CreatureParametersSPtr creature = container_.getCreatureParameters(creatureIndex);
            sf::CircleShape circle;
            float radius = creature->weight_ / 200.f; //alert MAGIC
            circle.setPosition(creature->positionX_ - radius, creature->positionY_ - radius);
            circle.setRadius(radius);
            Map::HSVvals hsv(creature->hue_, 1, 1);
            auto rgb = map_->convert2RGB(hsv);
            circle.setFillColor(sf::Color(rgb.r_, rgb.g_, rgb.b_, 200));
            window->draw(circle);
            sf::CircleShape marker;
            marker.setPosition(
                creature->positionX_ - radius * sin(creature->heading_) - radius / 10.f, //alert MAGIC
                creature->positionY_ - radius * cos(creature->heading_) - radius / 10.f);
            marker.setRadius(radius / 10.f); //alert MAGIC
            Map::HSVvals hsvmarker(creature->hue_, 1, 1);
            hsvmarker.h_ = fmod(hsvmarker.h_ + 180.f, 360.f);
            auto rgbmarker = map_->convert2RGB(hsvmarker);
            marker.setFillColor(sf::Color(rgbmarker.r_, rgbmarker.g_, rgbmarker.b_, 200));
            window->draw(marker);
        }
    }
}

void Simulation::printClipped_PROTO(std::shared_ptr<sf::RenderWindow> window, sf::View view)
{
    float xMin, xMax, yMin, yMax;
    xMin = view.getCenter().x - view.getSize().x / 2.f;
    xMax = view.getCenter().x + view.getSize().x / 2.f;
    yMin = view.getCenter().y - view.getSize().y / 2.f;
    yMax = view.getCenter().y + view.getSize().y / 2.f;
    for (int i = 0; i < data_PROTO_.size(); i += 24)
    {
        float xPos = data_PROTO_[i];
        float yPos = data_PROTO_[i + 1];
        if (xMin <= xPos && xPos <= xMax && yMin <= yPos && yPos <= yMax)
        {
            sf::CircleShape circle;
            circle.setPosition(data_PROTO_[i] - data_PROTO_[i + 4] / (20.f * 2.f), data_PROTO_[i + 1] - data_PROTO_[i + 4] / (20.f * 2.f));
            circle.setRadius(data_PROTO_[i + 4] / 20.f);
            circle.setFillColor(sf::Color(data_PROTO_[i + 5] * .5f, data_PROTO_[i + 6] * .5f, data_PROTO_[i + 7] * .5f));
            window->draw(circle);
        }
    }
};

bool Simulation::tryNewData()
{
    return dataSemaphore_.try_wait();
}

void Simulation::postVideo()
{
    while (dataSemaphore_.try_wait())
    {
    }
    dataSemaphore_.post();
}

void Simulation::setMap(shared_ptr<Map> mapPtr)
{
    map_ = mapPtr;
}