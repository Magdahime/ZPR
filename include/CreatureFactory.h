/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Singleton Creature Factory                    ///
///                                                   ///
///                                                   ///
/////////////////////////////////////////////////////////

#pragma once
#include <iostream>
#include <map>
#include <string>
#include <memory>
#include "Creature.h"
#include <boost/json.hpp>


const std::string DEFAULT_CREATURE_PATH = ".\\config\\creature";
struct CreatureParameters;

using CreatureParametersSPtr = std::shared_ptr<CreatureParameters>;
using FactoryMap = std::map<std::string, CreatureParametersSPtr>;

class CreatureFactory
{

    CreatureFactory();
    CreatureFactory(const CreatureFactory &) = delete;
    CreatureFactory &operator=(const CreatureFactory &) = delete;
    FactoryMap FactoryMap_;
    CreatureParametersSPtr parseCreature(boost::json::object obj);

public:
    static CreatureFactory &getInstance();
    void registerCreature();
    void registerFolder(std::string pathToFolder);
    FactoryMap getFactoryMap() { return FactoryMap_; }
    ~CreatureFactory() { FactoryMap_.clear(); }
};

struct CreatureParameters
{
    std::string type_;
    float energy_;
    float weight_;
    float hue_;
    float positionX_;
    float positionY_;
    float speedMultiplier_;
    CreatureParameters(std::string type, float energy, float weight, float hue, float positionX, float positionY,
                       float speedMultiplier) : type_(type), energy_(energy), weight_(weight), hue_(hue), positionX_(positionX),
                                                positionY_(positionY), speedMultiplier_(speedMultiplier){};
};
