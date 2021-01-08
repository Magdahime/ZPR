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
        boost::json::object obj = JsonParser::getValueFromFile(entry);
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
    CreatureParametersSPtr csptrCopy = std::make_shared<CreatureParameters>(*csptr);
    thread_local static RandomNumberGenerator<float, float> rng;
    csptrCopy->age_ = 0.f;
    csptrCopy->energy_ = fmax(0, rng.getNormal(csptr->energy_, CREATURE_FACTORY_STDDEV));
    csptrCopy->weight_ = fmax(0, rng.getNormal(csptr->weight_, CREATURE_FACTORY_STDDEV));
    csptrCopy->hue_ = fmod(rng.getNormal(csptr->hue_, CREATURE_FACTORY_STDDEV) + 360.f, 360.f);
    csptrCopy->positionX_ = rng.getNormal(csptr->positionX_, CREATURE_FACTORY_STDDEV);
    csptrCopy->positionY_ = rng.getNormal(csptr->positionY_, CREATURE_FACTORY_STDDEV);
    csptrCopy->speedMultiplier_ = fabs(rng.getNormal(csptr->speedMultiplier_, CREATURE_FACTORY_STDDEV));
    csptrCopy->heading_ = rng.get(MIN_CREATURE_HEADING, MAX_CREATURE_HEADING);
    csptrCopy->speed_ = rng.get(MIN_CREATURE_SPEED, MAX_CREATURE_SPEED);
    return csptrCopy;
}

CreatureParametersSPtr CreatureFactory::createChild(CreatureParametersSPtr csptr)
{
    return create(csptr);
}
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