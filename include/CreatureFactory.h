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

const std::string DEFAULT_CONFIGURATION_PATH = ".\\config\\creature";
const std::string DEFAULT_CREATURE = "default";

const float STDDEV = 0.5;

struct CreatureParameters;

using CreatureParametersSPtr = std::shared_ptr<CreatureParameters>;
using FactoryMap = std::map<std::string, CreatureParametersSPtr>;

class CreatureFactory
{
    FactoryMap FactoryMap_;

    CreatureFactory();
    CreatureFactory(const CreatureFactory &) = delete;
    CreatureFactory &operator=(const CreatureFactory &) = delete;
    CreatureParametersSPtr parseCreature(boost::json::object obj);
    CreatureParametersSPtr create(CreatureParametersSPtr csptr);

public:
    CreatureParametersSPtr createCreature(std::string type);
    CreatureParametersSPtr createChild(CreatureParametersSPtr csptr);
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
    float age_;
    float heading_;
    float speed_;
    float leftAntenna_;
    float rightAntenna_;
    float rearAntenna_;
    float bottomAntenna_;

    CreatureParameters(
        std::string type,
        float energy,
        float weight,
        float hue,
        float positionX,
        float positionY,
        float speedMultiplier,
        float age = .0f,
        float heading = .0f,
        float speed = .0f,
        float leftAntenna = .0f,
        float rightAntenna = .0f,
        float rearAntenna = .0f,
        float bottomAntenna = .0f) : type_(type),
                                     energy_(energy),
                                     weight_(weight),
                                     hue_(hue),
                                     positionX_(positionX),
                                     positionY_(positionY),
                                     speedMultiplier_(speedMultiplier),
                                     age_(age),
                                     heading_(heading),
                                     speed_(speed),
                                     leftAntenna_(leftAntenna),
                                     rightAntenna_(rightAntenna),
                                     rearAntenna_(rearAntenna),
                                     bottomAntenna_(bottomAntenna){};
};
