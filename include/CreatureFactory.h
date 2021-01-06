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

const float CREATURE_FACTORY_STDDEV = 2.0;
const unsigned int PARAMS_PER_CREATURE = 14;

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
    CreatureParametersSPtr createCreature(const std::string &type);
    CreatureParametersSPtr createChild(CreatureParametersSPtr csptr);
    static CreatureFactory &getInstance();
    void registerCreature(const std::string &pathToFile);
    void registerFolder(const std::string &pathToFolder);
    FactoryMap getFactoryMap() { return FactoryMap_; }
    ~CreatureFactory() { FactoryMap_.clear(); }
};

struct CreatureParameters
{
    float energy_;
    float weight_;
    float hue_;
    float positionX_;
    float positionY_;
    float speedMultiplier_;
    float age_;
    float heading_;
    float speed_;
    float leftAntennaH_;
    float rightAntennaH_;
    float rearAntennaH_;
    float bottomAntennaH_;
    float popDensity_;
    std::string type_;

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
        float leftAntennaH = .0f,
        float rightAntennaH = .0f,
        float rearAntennaH = .0f,
        float bottomAntennaH = .0f,
        float popDensity = .0f) : type_(type),
                                  energy_(energy),
                                  weight_(weight),
                                  hue_(hue),
                                  positionX_(positionX),
                                  positionY_(positionY),
                                  speedMultiplier_(speedMultiplier),
                                  age_(age),
                                  heading_(heading),
                                  speed_(speed),
                                  leftAntennaH_(leftAntennaH),
                                  rightAntennaH_(rightAntennaH),
                                  rearAntennaH_(rearAntennaH),
                                  bottomAntennaH_(bottomAntennaH),
                                  popDensity_(popDensity){};
};
