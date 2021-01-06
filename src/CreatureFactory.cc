#include <filesystem>
#include "CreatureFactory.h"
#include "JsonParser.h"
#include "RandomNumberGenerator.h"

namespace fs = std::filesystem;
void CreatureFactory::registerFolder(const std::string &pathToFolder)
{
    std::vector<std::filesystem::path> files = JsonParser::searchFiles(pathToFolder);
    for (auto entry : files)
    {
        boost::json::object &obj = JsonParser::getValueFromFile(entry);
        CreatureParametersSPtr csptr = parseCreature(obj);
        FactoryMap_.insert(std::pair<std::string, CreatureParametersSPtr>(csptr->type_, csptr));
    }
}

void CreatureFactory::registerCreature(const std::string &pathToFile)
{
    std::filesystem::path path = pathToFile;
    boost::json::object &obj = JsonParser::getValueFromFile(path);
    CreatureParametersSPtr csptr = parseCreature(obj);
    FactoryMap_.insert(std::pair<std::string, CreatureParametersSPtr>(csptr->type_, csptr));
}

CreatureParametersSPtr CreatureFactory::parseCreature(boost::json::object obj)
{
    float energy = obj["energy"].is_double() ? (float)obj["energy"].as_double() : (float)obj["energy"].as_int64();
    float weight = obj["weight"].is_double() ? (float)obj["weight"].as_double() : (float)obj["weight"].as_int64();
    float hue = obj["hue"].is_double() ? (float)obj["hue"].as_double() : (float)obj["hue"].as_int64();
    float positionX = obj["positionX"].is_double() ? (float)obj["positionX"].as_double() : (float)obj["positionX"].as_int64();
    float positionY = obj["positionY"].is_double() ? (float)obj["positionY"].as_double() : (float)obj["positionY"].as_int64();
    float speedMultiplier = obj["speedMultiplier"].is_double() ? (float)obj["speedMultiplier"].as_double() : (float)obj["speedMultiplier"].as_int64();
    return std::make_shared<CreatureParameters>(std::string(obj["type"].as_string().c_str()), energy,
                                                             weight, hue, positionX, positionY,
                                                             speedMultiplier);
}

CreatureFactory &CreatureFactory::getInstance()
{
    static CreatureFactory instance;
    return instance;
}

CreatureFactory::CreatureFactory()
{
    registerFolder(DEFAULT_CONFIGURATION_PATH);
}

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

CreatureParametersSPtr CreatureFactory::create(CreatureParametersSPtr csptr)
{
    thread_local static RandomNumberGenerator<float, float> rng;
    csptr->age_ = 0.f;
    csptr->energy_ = fmax(0, rng.getNormal(csptr->energy_, CREATURE_FACTORY_STDDEV));
    csptr->weight_ = fmax(0, rng.getNormal(csptr->weight_, CREATURE_FACTORY_STDDEV));
    csptr->hue_ = fmod(rng.getNormal(csptr->hue_, CREATURE_FACTORY_STDDEV) + 360.f, 360.f);
    csptr->positionX_ = rng.getNormal(csptr->positionX_, CREATURE_FACTORY_STDDEV);
    csptr->positionY_ = rng.getNormal(csptr->positionY_, CREATURE_FACTORY_STDDEV);
    csptr->speedMultiplier_ = fabs(rng.getNormal(csptr->speedMultiplier_, CREATURE_FACTORY_STDDEV));
    csptr->heading_ = rng.get(MIN_CREATURE_HEADING, MAX_CREATURE_HEADING);
    csptr->speed_ = rng.get(MIN_CREATURE_SPEED, MAX_CREATURE_SPEED);
    return csptr;
}

CreatureParametersSPtr CreatureFactory::createChild(CreatureParametersSPtr csptr)
{
    return create(csptr);
}