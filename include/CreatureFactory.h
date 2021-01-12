/**
 * \file
 * \author Magdalena Majkowska
 */

#pragma once

#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <iostream>
#include <map>
#include <string>
#include <memory>
#include <vector>
#include <filesystem>

#include <boost/json.hpp>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

#include "Creature.h"

namespace zpr
{
    const std::string DEFAULT_CREATURE = "default";
    const std::vector<std::string> CONFIG_PATH = {"config", "creature"};

    const float CREATURE_FACTORY_STDDEV = 2.0;
    const unsigned int PARAMS_PER_CREATURE = 14;

    //Forward declaration
    struct CreatureParameters;

    using CreatureParametersSPtr = std::shared_ptr<CreatureParameters>;
    using FactoryMap = std::map<std::string, CreatureParametersSPtr>;
    /**
     * \author Magdalena Majkowska
     * 
     * This is the singleton Creature Factory, which allows for creation of respective CreatureParameters
     * based on the registered JSON configurations. CreatureParameters are later inputted into
     * \link CreatureContainer and used in the Simulation
     */
    class CreatureFactory
    {
        FactoryMap FactoryMap_;

        /**
        * Constructor of CreatureFactory - it automatically registers all creatures from default path.
        */
        CreatureFactory();

        CreatureFactory(const CreatureFactory &) = delete;
        CreatureFactory(CreatureFactory &&) = delete;
        CreatureFactory &operator=(CreatureFactory &&) = delete;
        CreatureFactory &operator=(const CreatureFactory &) = delete;

        /**
        * Private method for creating a creature. It takes all of the parameters of the specimen 
        * and changes them accordingly with normal distribution around this value. 
        *\return shared pointer to the new CreatureParameters
        */
        CreatureParametersSPtr create(CreatureParametersSPtr csptr);

        std::string configurationPath_;

    public:
        /**
        * Parses creature's parameters from JSON files. It uses a BOOST JSON library, that was released in 1.75.0
        * BOOST version. As we can see here there was a little problem with parsing doubles - numbers without a
        * fractional part was not read correctly fom object, so we had to use the casting methods. 
        * \param obj BOOST JSON object
        */
        CreatureParametersSPtr parseCreature(boost::json::object obj);
        /**
        * Public method for creating a Creature. Firstly it checks the map if this specimen is registered,
        * if it's not it creates a default creature. Otherwise it creates the one specified in the type 
        * parameter.
        *\param type unique string that defines a specimen of a creature
        */
        CreatureParametersSPtr createCreature(const std::string &type);
        /**
        * It creates a child creature based on the parameters of the parent.
        *\param csptr shared pointer to the parent parameters 
        *\return shared pointer to the new CreatureParameters
        */
        CreatureParametersSPtr createChild(CreatureParametersSPtr csptr);
        /**
        * Method of getting an instance of CreatureFactory.
        * \return a static instance of CreatureFactory
        */
        static CreatureFactory &getInstance();
        /**
        * Method to create a JSON string, containing all the available specimens. 
        * Used for communication between webview and C++ programme.
        *\return JSON string with the names of the specimens.
        */
        std::string parseKeys();
        /**
        * Method to create a JSON string, containing all of the information
        * about selected specimen. 
        * Used for communication between webview and C++ programme.
        *\return JSON string with the data about specimen.
        */
        std::string getParsedValues(std::string key);
        /**
        * Similar method to registerFolder but it permits you to register only one creature. It is necessary for
        * runtime adding new types of creatures. 
        * \param pathToFile path to JSON file
        */
        void registerCreature(const std::string &pathToFile);
        /**
        * Method used to register an entire folder with JSON files, that defines new creature parameters.
        * \param pathToFolder path to folder
        */
        void registerFolder(const std::string &pathToFolder);
        FactoryMap getFactoryMap() { return FactoryMap_; }
        std::string getConfigurationPath() { return configurationPath_ + std::filesystem::path::preferred_separator; };
        ~CreatureFactory() { FactoryMap_.clear(); }
    };

    /**
     * CreatureParameters represent the complete set of features that define a single specimen.
     * They are also used as templates for CreatureFactory
     */
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
            const std::string &type,
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
            float popDensity = .0f) : energy_(energy),
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
                                      popDensity_(popDensity),
                                      type_(type){};
    };
}; // namespace zpr