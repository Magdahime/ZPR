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
    // #pragma omp parallel for reduction(+ : counter)
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
            creature->heading_ += ANGLE_PER_FRAME * 2.f * pi / 360.f;
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
            creature->speed_ *= ACCELERATION_MULTIPLIER;
            creature->speed_ = fmin(creature->speed_, MAX_SPEED);
        }
        else
        {
            creature->speed_ /= ACCELERATION_MULTIPLIER;
        }
    }
    if (results[2] > ACTIVATION_THRESHOLD) // EAT
    {
        float currH = map_->getPixelH(creature->positionX_, creature->positionY_);
        float diffH = fabs(creature->hue_ - currH);
        if (diffH > 180.f)
            diffH = fabs(diffH - 360.f);
        float dist = -((diffH * diffH + 30.f * 30.f) / 180.f - 50.f); //alert MAGIC
        creature->energy_ += dist;
    }
    bool birth = false;
    if (results[4] > ACTIVATION_THRESHOLD && creature->age_ > BIRTH_AGE_THRESHOLD) // BIRTH
    {
        thread_local static RandomNumberGenerator<float> rng;
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
    creature->speed_ = fabs(creature->speed_);
    float movement = creature->speed_ * creature->speedMultiplier_;
    if (creature->energy_ > ENERGY_THRESHOLD)
    {
        creature->weight_ += (creature->energy_ - ENERGY_THRESHOLD);
        creature->energy_ = ENERGY_THRESHOLD;
    }
    else if (creature->energy_ < 0)
    {
        creature->weight_ -= fabs(creature->energy_);
        creature->energy_ = 0;
    }

    creature->positionX_ += sin(creature->heading_) * movement;
    (creature->positionX_ > map_->getWidth()) ? creature->positionX_ -= map_->getWidth() : ((creature->positionX_ < 0) ? creature->positionX_ += map_->getWidth() : 0);
    creature->positionY_ += cos(creature->heading_) * movement;
    (creature->positionY_ > map_->getHeight()) ? creature->positionY_ -= map_->getHeight() : ((creature->positionY_ < 0) ? creature->positionY_ += map_->getHeight() : 0);
    creature->bottomAntennaH_ = map_->getPixelH(creature->positionX_, creature->positionY_);

    float xPos = creature->positionX_ + creature->speedMultiplier_ * sin(creature->heading_ - 45.f / 180.f * pi);
    float yPos = creature->positionY_ + creature->speedMultiplier_ * cos(creature->heading_ - 45.f / 180.f * pi);
    float hValue = map_->getPixelH(xPos, yPos);
    creature->rightAntennaH_ = (hValue != INVALID_COORDS) ? hValue : creature->bottomAntennaH_;

    xPos = creature->positionX_ + creature->speedMultiplier_ * sin(creature->heading_ + 45.f / 180.f * pi);
    yPos = creature->positionY_ + creature->speedMultiplier_ * cos(creature->heading_ + 45.f / 180.f * pi);
    hValue = map_->getPixelH(xPos, yPos);
    creature->leftAntennaH_ = (hValue != INVALID_COORDS) ? hValue : creature->bottomAntennaH_;

    xPos = creature->positionX_ - creature->speedMultiplier_ * sin(creature->heading_);
    yPos = creature->positionY_ - creature->speedMultiplier_ * cos(creature->heading_);
    hValue = map_->getPixelH(xPos, yPos);
    creature->rearAntennaH_ = (hValue != INVALID_COORDS) ? hValue : creature->bottomAntennaH_;

    creature->age_ += 1.f / TARGET_FPS;

    if (birth)
    {
        auto childParams = CreatureFactory::getInstance().createChild(creature);
        auto neurons = container_.getNeurons(creatureIndex);
        auto childNeurons = NeuronFactory::getInstance().createChild(neurons);
        // std::cout<<"\nStarting birth...\n"; //alert DEBUG COUT
        container_.putCreature(childParams, childNeurons);
    }
    if (creature->weight_ < MIN_WEIGHT)
    {
        container_.deleteCreature(creatureIndex);
    }

    container_.updateCreatureParameters(creatureIndex, creature);
}

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
                creature->positionX_ + radius * sin(creature->heading_) - radius / 10.f, //alert MAGIC
                creature->positionY_ + radius * cos(creature->heading_) - radius / 10.f);
            marker.setRadius(radius / 10.f); //alert MAGIC
            Map::HSVvals hsvmarker(creature->hue_, 1, 1);
            hsvmarker.h_ = fmod(hsvmarker.h_ + 180.f, 360.f);
            auto rgbmarker = map_->convert2RGB(hsvmarker);
            marker.setFillColor(sf::Color(rgbmarker.r_, rgbmarker.g_, rgbmarker.b_, 200));
            window->draw(marker);
        }
    }
}

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