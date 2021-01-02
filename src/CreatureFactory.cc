#include <filesystem>
#include "CreatureFactory.h"
#include "JsonParser.h"
#include "RandomNumberGenerator.h"

namespace fs = std::filesystem;
void CreatureFactory::registerFolder(std::string pathToFolder)
{
    std::vector<std::filesystem::path> files = JsonParser::searchFiles(pathToFolder);
    for (auto entry : files)
    {
        boost::json::object &obj = JsonParser::getValueFromFile(entry);
        CreatureParametersSPtr csptr = parseCreature(obj);
        FactoryMap_.insert(std::pair<std::string, CreatureParametersSPtr>(csptr->type_, csptr));
    }
}

CreatureParametersSPtr CreatureFactory::parseCreature(boost::json::object obj)
{
    return std::make_shared<CreatureParameters>(std::string(obj["type"].as_string().c_str()),
                                                (float)obj["energy"].as_double(),
                                                (float)obj["weight"].as_double(),
                                                (float)obj["hue"].as_double(),
                                                (float)obj["positionX"].as_double(),
                                                (float)obj["positionY"].as_double(),
                                                (float)obj["positionY"].as_double());
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

CreatureParametersSPtr CreatureFactory::createCreature(std::string type)
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
    RandomNumberGenerator<float, float> rng;
    csptr->energy_ = rng.getNormal(csptr->energy_, STDDEV);
    csptr->weight_ = rng.getNormal(csptr->weight_, STDDEV);
    csptr->hue_ = rng.getNormal(csptr->hue_, STDDEV);
    csptr->positionX_ = rng.getNormal(csptr->positionX_, STDDEV);
    csptr->positionY_ = rng.getNormal(csptr->positionY_, STDDEV);
    csptr->speedMultiplier_ = rng.getNormal(csptr->speedMultiplier_, STDDEV);
    csptr->heading_ = rng.get(MIN_CREATURE_HEADING, MAX_CREATURE_HEADING);
    csptr->speed_ = rng.get(MIN_CREATURE_SPEED, MAX_CREATURE_SPEED);
    return csptr;
}

CreatureParametersSPtr CreatureFactory::createChild(CreatureParametersSPtr csptr){
    return create(csptr);
}