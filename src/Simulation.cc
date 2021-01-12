#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <iostream>
#include <cmath>
#include <chrono>
#include <thread>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <boost/json.hpp>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

#include <omp.h>

#include "Simulation.h"
#include "Creature.h"
#include "Neuron.h"
#include "Map.h"

Simulation::Simulation() : dataSemaphore_(1), videoSemaphore_(0){};

void Simulation::prepare(size_t creatureCount)
{
    for (size_t i = 0; i < creatureCount; ++i)
    {
        container_.putCreature();
    }
}
void Simulation::prepare()
{
    prepare(parameters_.creaturesNum_);
}
void Simulation::run()
{
    prepare();
    int i = 0;
    time_t now = time(0);
    time_t newnow;
    while (!terminate_)
    {
        container_.setDivisors(parameters_.energyThreshhold_ / 2.f, fmax(1.f, avgWeight_), fmax(1.f, avgAge_), parameters_.energyThreshhold_ / 2.f);
        dataSemaphore_.wait();
        if (terminate_)
            break;
        i++;
        newnow = time(0);
        /**
         * Iterations per second counter
         */
        if (newnow != now)
        {
            std::cout << "\nIPS:\t" << i << "\t CCount:\t" << populationSize_ << "\n";
            i = 0;
        }
        now = newnow;
        populationSize_ = iteration();
        videoSemaphore_.post();
    }
}
int Simulation::iteration()
{
    std::lock_guard<std::mutex> lockGuard(iterationMutex_);
    ++iterationNumber_;
    int creatureCounter = 0;
    float totalAge = 0.f;
    float totalWeight = 0.f;
#pragma omp parallel for reduction(+ : creatureCounter, totalWeight, totalAge)
    for (size_t creatureIndex = 0; creatureIndex < container_.getSize(); ++creatureIndex)
    {
        if (container_.isDeleted(creatureIndex))
            continue;
        /**
         * OpenMP happily allows for reducing sums over whole workgroup. 
         * The specification allows for reducing maximums/minimums, but this remains unimplemented
         * in MSVC and thus we had to ommit it.
         */
        ++creatureCounter;
        totalAge += container_.getCreatureValue(creatureIndex, CONTAINER_SLOT_AGE);
        totalWeight += container_.getCreatureValue(creatureIndex, CONTAINER_SLOT_WEIGHT);

        /**
         * Populating neurons before NN calculations
         */
        container_.populateNeurons(creatureIndex);

        /**
         * Neural Network calculations
         */
        for (unsigned int layerIndex = 0; layerIndex < LAYER_WIDTHS.size(); layerIndex++)
        {
            container_.calculateLayer(creatureIndex, layerIndex);
        }

        updateCreature(creatureIndex);
    }
    avgAge_ = totalAge / creatureCounter;
    avgWeight_ = totalWeight / creatureCounter;
    container_.putQueue();
    return creatureCounter;
}

void Simulation::updateCreature(size_t creatureIndex)
{

    std::vector<float> results = container_.getResult(creatureIndex); // 0 -> steer
                                                                      // 1 -> accelerate
                                                                      // 2 -> eat
                                                                      // 3 -> attack
                                                                      // 4 -> birth
    CreatureParametersSPtr creature = container_.getCreatureParameters(creatureIndex);

    /**
     * Call on all possible actions to be calculated and processed
     */
    calculateSteer(creature, results[0]);
    calculateAcceleration(creature, results[1]);
    calculateEating(creature, results[2]);
    calculateAttack(creature, results[3]);
    bool birth = calculateBirth(creature, results[4]);
    calculateEnergy(creature);
    calculateMovement(creature);
    calculateAntennas(creature);
    calculateAge(creature);

    /**
     * Birth handling.
     * If a new creature is birthed, the fact is delayed until after the whole iteration is done.
     */
    if (birth)
    {
        auto childParams = CreatureFactory::getInstance().createChild(creature);
        childParams->weight_ = parameters_.weightBirth_;
        auto neurons = container_.getNeurons(creatureIndex);
        auto childNeurons = NeuronFactory::getInstance().createChild(neurons);
        container_.delayPutCreature(childParams, childNeurons);
    }
    if (creature->weight_ < parameters_.minWeight_)
    {
        container_.deleteCreature(creatureIndex);
        if(creatureIndex == selectedIndex_)
            unselect();
    }

    container_.updateCreatureParameters(creatureIndex, creature);
}

void Simulation::calculateSteer(CreatureParametersSPtr creature, float result)
{
    if (fabs(result) > ACTIVATION_THRESHOLD)
    {
        if (result > 0)
        {
            creature->heading_ += parameters_.anglePerFrame_ * 2.f * pi / 360.f;
            creature->heading_ = fmod(creature->heading_ + 2.f * pi, 2.f * pi);
        }
        else
        {
            creature->heading_ -= 2.f * 2.f * pi / 360.f;
            creature->heading_ = fmod(creature->heading_ + 2.f * pi, 2.f * pi);
        }
    }
}

void Simulation::calculateAcceleration(CreatureParametersSPtr creature, float result)
{
    if (fabs(result) > ACTIVATION_THRESHOLD) // ACCELERATE
    {
        if (result > 0)
        {
            creature->speed_ *= parameters_.accelerationMultiplier_;
            creature->speed_ = fmin(creature->speed_, parameters_.maxSpeed_);
        }
        else
        {
            creature->speed_ /= parameters_.accelerationMultiplier_;
            creature->speed_ = fmin(creature->speed_, parameters_.maxSpeed_);
        }
        creature->speed_ = fmax(creature->speed_, MIN_SPEED);
    }
}

void Simulation::calculateEating(CreatureParametersSPtr creature, float result)
{

    if (result > ACTIVATION_THRESHOLD) // EAT
    {
        float currH = map_->getPixelH(creature->positionX_, creature->positionY_);
        float diffH = fabs(creature->hue_ - currH);
        if (diffH > 180.f)
            diffH = fabs(diffH - 360.f);
        /**
         * This function calculates energy loss/gain on eating based on difference between
         * creature's and surface's color.
         * 
         * See: https://www.wolframalpha.com/input/?i=-%28%28x+*+x+%2B+30+*+30%29+%2F+180+-+50%29+for+x+from+0+to+180
         */
        float dist = -((diffH * diffH + 30.f * 30.f) / 180.f - 50.f);
        creature->energy_ += dist;
    }
}
void Simulation::calculateAttack(CreatureParametersSPtr creature, float result) { (void)creature; (void)result; } //Supressing -Wunused-parameter

bool Simulation::calculateBirth(CreatureParametersSPtr creature, float result)
{
    bool birth = false;
    if (result > ACTIVATION_THRESHOLD && creature->age_ > parameters_.birthAgeThreshhold_) // BIRTH
    {
        thread_local static RandomNumberGenerator<float> rng;
        if (creature->weight_ + rng.get(0, parameters_.birthWeightThreshhold_) > parameters_.birthWeightThreshhold_)
        {
            birth = true;
            /**
             * Birth penalty
             */
            creature->energy_ -= parameters_.energyBirth_;
            creature->weight_ -= parameters_.weightBirth_;
        }
        else
        {
            /**
             * Unsuccesful birth penalty
             */
            creature->energy_ -= parameters_.energyBirthFailed_;
        }
    }
    return birth;
}

void Simulation::calculateMovement(CreatureParametersSPtr creature)
{
    creature->speed_ = fabs(creature->speed_);
    float movement = fmin(creature->speed_ * creature->speedMultiplier_, parameters_.maxSpeed_);
    creature->positionX_ += sin(creature->heading_) * movement;
    creature->positionX_ = fmod(creature->positionX_ + map_->getWidth(), map_->getWidth());
    creature->positionY_ += cos(creature->heading_) * movement;
    creature->positionY_ = fmod(creature->positionY_ + map_->getHeight(), map_->getHeight());
}
void Simulation::calculateAntennas(CreatureParametersSPtr creature)
{
    creature->bottomAntennaH_ = map_->getPixelH(creature->positionX_, creature->positionY_);

    /**
     * If any of the antennae outside of map, values considered the same as of the bottom one
     */
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
}

void Simulation::calculateAge(CreatureParametersSPtr creature)
{
    creature->age_ += 1.f / zpr_windows::TARGET_FPS;
}

void Simulation::calculateEnergy(CreatureParametersSPtr creature)
{
    /**
     * The following calculates idle energy consumption, increasing with age
     */
    creature->energy_ -= (100.f + creature->age_) / 400.f;
    if (creature->energy_ > parameters_.energyThreshhold_)
    {
        creature->weight_ += parameters_.weightGained_;
        creature->energy_ = 0.0f;
    }
    else if (creature->energy_ < 0)
    {
        creature->weight_ -= parameters_.weightLost_;
        creature->energy_ = 0.0f;
    }
}

void Simulation::printAll(std::shared_ptr<sf::RenderWindow> window)
{
    for (size_t creatureIndex = 0; creatureIndex < container_.getSize(); ++creatureIndex)
    {
        CreatureParametersSPtr creature = container_.getCreatureParameters(creatureIndex);
        sf::CircleShape circle;
        circle.setPosition(creature->positionX_ - creature->weight_ / 2.0f, creature->positionY_ - creature->weight_ / 2.0f);
        circle.setRadius(creature->weight_ / 2.0f);
        HSVvals hsv(creature->hue_, 1, 1);
        auto rgb = convert2RGB(hsv);
        circle.setFillColor(sf::Color(rgb.r_, rgb.g_, rgb.b_));
        window->draw(circle);
    }
}

void Simulation::printClipped(std::shared_ptr<sf::RenderWindow> window, sf::View view)
{
    float xMin, xMax, yMin, yMax;
    /**
     * Calculating clipping coordinates
     */
    xMin = view.getCenter().x - view.getSize().x / 2.f;
    xMax = view.getCenter().x + view.getSize().x / 2.f;
    yMin = view.getCenter().y - view.getSize().y / 2.f;
    yMax = view.getCenter().y + view.getSize().y / 2.f;
    for (size_t creatureIndex = 0; creatureIndex < container_.getSize(); ++creatureIndex)
    {
        if (container_.isDeleted(creatureIndex))
            continue;
        float xPos = container_.getCreatureX(creatureIndex);
        float yPos = container_.getCreatureY(creatureIndex);
        if (xMin <= xPos && xPos <= xMax && yMin <= yPos && yPos <= yMax)
        {
            CreatureParametersSPtr creature = container_.getCreatureParameters(creatureIndex);
            sf::CircleShape circle;
            float radius = fabs(creature->weight_) / 20.f; // Radius has to be a bit smaller than weight
            circle.setPosition(creature->positionX_ - radius, creature->positionY_ - radius);
            circle.setRadius(radius);
            HSVvals hsv(creature->hue_, 1, 1);
            auto rgb = convert2RGB(hsv);
            circle.setFillColor(sf::Color(rgb.r_, rgb.g_, rgb.b_, 200));
            window->draw(circle);
            sf::CircleShape marker;
            // Direction marker is 1/10th size of the creature itself
            marker.setPosition(
                creature->positionX_ + radius * sin(creature->heading_) - radius / 10.f,
                creature->positionY_ + radius * cos(creature->heading_) - radius / 10.f);
            marker.setRadius(radius / 10.f);
            HSVvals hsvmarker(creature->hue_, 1, 1);
            hsvmarker.h_ = fmod(hsvmarker.h_ + 180.f, 360.f);
            auto rgbmarker = convert2RGB(hsvmarker);
            marker.setFillColor(sf::Color(rgbmarker.r_, rgbmarker.g_, rgbmarker.b_, 200));
            window->draw(marker);
            if (creatureIndex == selectedIndex_)
            {
                sf::RectangleShape selected;
                selected.setPosition(creature->positionX_ - 1.f, creature->positionY_ - 1.f);
                selected.setSize(sf::Vector2f(2.f, 2.f));
                selected.setFillColor(sf::Color(0, 0, 0, 255));
                window->draw(selected);
            }
        }
    }
}

void Simulation::postVideo()
{
    dataSemaphore_.post();
}

void Simulation::waitVideo()
{
    videoSemaphore_.wait();
}

void Simulation::setMap(std::shared_ptr<Map> mapPtr)
{
    map_ = mapPtr;
}

void Simulation::selectClosestCreature(float x, float y)
{
    /**
     * findClosestCreature should not ever lock up, so the detach is mostly safe
     */
    std::thread findThread(&Simulation::findClosestCreature, this, x, y);
    findThread.detach();
}

void Simulation::findClosestCreature(float x, float y)
{
    float minDist = INFINITY;
    size_t foundIndex = 0 - 1;
    for (size_t creatureIndex = 0; creatureIndex < container_.getSize(); ++creatureIndex)
    {
        if (container_.isDeleted(creatureIndex))
            continue;
        float xDiff = container_.getCreatureX(creatureIndex) - x;
        float yDiff = container_.getCreatureY(creatureIndex) - y;
        if (xDiff * xDiff + yDiff * yDiff < minDist)
        {
            minDist = xDiff * xDiff + yDiff * yDiff;
            foundIndex = creatureIndex;
        }
    }
    selectedIndex_ = foundIndex;
}

float Simulation::getSelectedX()
{
    if (isSelected())
    {
        return container_.getCreatureValue(selectedIndex_, 3);
    }
    return map_->getWidth() / 2;
}
float Simulation::getSelectedY()
{
    if (isSelected())
    {
        return container_.getCreatureValue(selectedIndex_, 4);
    }
    return map_->getHeight() / 2;
}

std::string Simulation::getSelectedParametersAsJSON()
{
    if(!isSelected()){
        return "None selected";
    }
    std::string comma = ", ";
    std::string quote = R"(")";
    auto params = container_.getCreatureParameters(selectedIndex_);
    // This is not the best way of JSON creation, but seemed the most practical before introducing Boost.JSON
    // after which we did not manage to rewrite this
    std::string out = R"('{"data":[)";
    out += std::to_string(params->age_) + comma;
    out += std::to_string(params->weight_) + comma;
    out += std::to_string(params->energy_) + comma;
    out += std::to_string(params->hue_) + comma;
    out += quote + std::to_string(params->positionX_) + comma + std::to_string(params->positionY_) + quote + comma;
    out += std::to_string(params->speed_) + comma;
    out += std::to_string(params->heading_) + comma;
    out += std::to_string(params->leftAntennaH_) + comma;
    out += std::to_string(params->rightAntennaH_) + comma;
    out += std::to_string(params->bottomAntennaH_);
    out += R"(]}')";
    return out;
};

std::string Simulation::getSelectedNeuronsAsJSON()
{
    if(!isSelected()){
        return "None selected";
    }
    auto neurons = container_.getNeuronStates(selectedIndex_);
    // This is not the best way of JSON creation, but seemed the most practical before introducing Boost.JSON
    // after which we did not manage to rewrite this
    std::string out = R"('{"nodes" : [)";
    for (size_t i = 0; i < neurons.size(); ++i)
    {
        for (size_t j = 0; j < neurons[i].size(); ++j)
        {
            out += std::string(R"({"label":)");
            out += std::to_string((neurons[i])[j]);
            out += std::string(R"(, "layer": )");
            out += std::to_string(i + 1);
            if (!(i == neurons.size() - 1 && j == neurons[i].size() - 1))
                out += std::string(R"(},)");
        }
    }
    out += std::string(R"(}]}')");
    return out;
};

void Simulation::unselect(){
    selectedIndex_ = static_cast<size_t>(0 - 1);
}

void Simulation::putCreature(std::string type, int creatureNum)
{
    for (int i = 0; i < creatureNum; ++i)
    {
        container_.putCreature(type);
    }
}
