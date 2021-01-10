#pragma warning(push, 0)

#include <filesystem>

#pragma warning(pop)

#include "CreatureFactory.h"
#include "JsonParser.h"
#include "RandomNumberGenerator.h"

namespace fs = std::filesystem;
/**
* Method used to register an entire folder with JSON files, that defines new creature parameters.
* \param pathToFolder - path to folder
*/
void CreatureFactory::registerFolder(const std::string &pathToFolder)
{
    std::vector<std::filesystem::path> files = JsonParser::searchFiles(pathToFolder);
    for (auto entry : files)
    {
        boost::json::object obj = JsonParser::getValueFromFile(entry);
        CreatureParametersSPtr csptr = parseCreature(obj);
        FactoryMap_.insert(std::pair<std::string, CreatureParametersSPtr>(csptr->type_, csptr));
    }
}
/**
* Similar method to registerFolder but it permits you to register only one creature. It is necessary for
* runtime adding new types of creatures. 
* \param pathToFile - path to JSON file
*/
void CreatureFactory::registerCreature(const std::string &pathToFile)
{
    std::filesystem::path path = pathToFile;
    boost::json::object &obj = JsonParser::getValueFromFile(path);
    CreatureParametersSPtr csptr = parseCreature(obj);
    FactoryMap_.insert(std::pair<std::string, CreatureParametersSPtr>(csptr->type_, csptr));
}
/**
* Parses creature's parameters from JSON files. It uses a BOOST JSON library, that was released in 1.75.0
* BOOST version. As we can see here there was a little problem with parsing doubles - numbers without a
* fractional part was not read correctly fom object, so we had to use the casting methods. 
* \param obj - BOOST JSON object
*/
CreatureParametersSPtr CreatureFactory::parseCreature(boost::json::object obj)
{
    float energy = obj["energy"].is_double() ? std::static_cast<float>(obj["energy"].as_double()) : std::static_cast<float>(obj["energy"].as_int64());
    float weight = obj["weight"].is_double() ? std::static_cast<float>(obj["weight"].as_double()) : std::static_cast<float>(obj["weight"].as_int64());
    float hue = obj["hue"].is_double() ? std::static_cast<float>(obj["hue"].as_double()) : std::static_cast<float>(obj["hue"].as_int64());
    float positionX = obj["positionX"].is_double() ? std::static_cast<float>(obj["positionX"].as_double()) : std::static_cast<float>(obj["positionX"].as_int64());
    float positionY = obj["positionY"].is_double() ? std::static_cast<float>(obj["positionY"].as_double()) : std::static_cast<float>(obj["positionY"].as_int64());
    float speedMultiplier = obj["speedMultiplier"].is_double() ? std::static_cast<float>(obj["speedMultiplier"].as_double()) : std::static_cast<float>(obj["speedMultiplier"].as_int64());
    return std::make_shared<CreatureParameters>(std::string(obj["type"].as_string().c_str()), energy,
                                                weight, hue, positionX, positionY,
                                                speedMultiplier);
}
/**
* Method of getting an instance of CreatureFactory.
* \return - a static instance of CreatureFactory
*/
CreatureFactory &CreatureFactory::getInstance()
{
    static CreatureFactory instance;
    return instance;
}

/**
* Constructor of CreatureFactory - it automatically registers all creatures from default path
* as set in DEFAULT_CONFIGURATION_PATH.
*/

CreatureFactory::CreatureFactory()
{
    registerFolder(DEFAULT_CONFIGURATION_PATH);
}

/**
* Public method for creating a Creature. Firstly it checks the map if this specimen is registered,
* if it's not it creates a default creature. Otherwise it creates the one specified in the type 
* parameter.
*\param type - unique string that defines a specimen of a creature
*/
CreatureParametersSPtr CreatureFactory::createCreature(const std::string &type)
{
    auto iterator = FactoryMap_.find(type);
    if (iterator != FactoryMap_.end())
    {
        return create(iterator->second);
    }
    else
    {
        CreatureParametersSPtr csptr = FactoryMap_.at(DEFAULT_CREATURE);
        return create(csptr);
    }
}
/**
* Private method for creating a creature. It takes all of the parameters of the specimen 
* and changes them accordingly with normal distribution around this value. 
*\return - shared pointer to the new CreatureParameters
*/
CreatureParametersSPtr CreatureFactory::create(CreatureParametersSPtr csptr)
{
    CreatureParametersSPtr csptrCopy = std::make_shared<CreatureParameters>(*csptr);
    thread_local static RandomNumberGenerator<float, float> rng;
    csptrCopy->age_ = 0.f;
    csptrCopy->energy_ = fmax(0, rng.getNormal(csptr->energy_, CREATURE_FACTORY_STDDEV));
    csptrCopy->weight_ = fmax(0, rng.getNormal(csptr->weight_, CREATURE_FACTORY_STDDEV));
    csptrCopy->hue_ = fmod(rng.getNormal(csptr->hue_, CREATURE_FACTORY_STDDEV) + 360.f, 360.f);
    csptrCopy->positionX_ = rng.getNormal(csptr->positionX_, CREATURE_FACTORY_STDDEV);
    csptrCopy->positionY_ = rng.getNormal(csptr->positionY_, CREATURE_FACTORY_STDDEV);
    csptrCopy->speedMultiplier_ = fabs(rng.getNormal(csptr->speedMultiplier_, CREATURE_FACTORY_STDDEV));
    csptrCopy->heading_ = fmod(rng.getNormal(csptr->heading_, CREATURE_FACTORY_STDDEV) + 360.f, 360.f);
    csptrCopy->speed_ = fabs(rng.getNormal(csptr->speed_, CREATURE_FACTORY_STDDEV));
    return csptrCopy;
}
/**
* It creates a child creature based on the parameters of the parent.
*\param csptr - shared pointer to the parent parameters 
*\return - shared pointer to the new CreatureParameters
*/
CreatureParametersSPtr CreatureFactory::createChild(CreatureParametersSPtr csptr)
{
    return create(csptr);
}

/**
* Method to create a JSON string, containing all the available specimens. 
* Used for communication between webview and C++ programme.
*\return - JSON string with the names of the specimens.
*/
std::string CreatureFactory::parseKeys()
{
    std::string parsedKeys = "['";
    parsedKeys += FactoryMap_.begin()->first;
    parsedKeys += "'";
    for (FactoryMap::iterator it = ++FactoryMap_.begin(); it != FactoryMap_.end(); ++it)
    {
        parsedKeys += ",";
        parsedKeys += "'";
        parsedKeys += it->first;
        parsedKeys += "'";
    }
    parsedKeys += "]";
    return parsedKeys;
}
/**
* Method to create a JSON string, containing all of the information
* about selected specimen. 
* Used for communication between webview and C++ programme.
*\return - JSON string with the data about specimen.
*/
std::string CreatureFactory::getParsedValues(std::string key)
{
    CreatureParametersSPtr csptr = FactoryMap_[key];
    std::string parsed = "'{ ";
    parsed += R"("type":")";
    parsed += csptr->type_;
    parsed += R"(")";
    parsed += ",";
    parsed += R"("energy":)";
    parsed += std::to_string(csptr->energy_);
    parsed += ",";
    parsed += R"("weight":)";
    parsed += std::to_string(csptr->weight_);
    parsed += ",";
    parsed += R"("hue":)";
    parsed += std::to_string(csptr->hue_);
    parsed += ",";
    parsed += R"("positionX":)";
    parsed += std::to_string(csptr->positionX_);
    parsed += ",";
    parsed += R"("positionY":)";
    parsed += std::to_string(csptr->positionY_);
    parsed += ",";
    parsed += R"("speedMultiplier":)";
    parsed += std::to_string(csptr->speedMultiplier_);
    parsed += "}'";
    std::cout << parsed << std::endl;
    return parsed;
}