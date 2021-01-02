#include <filesystem>
#include "CreatureFactory.h"
#include "JsonParser.h"


namespace fs = std::filesystem;
void CreatureFactory::registerFolder(std::string pathToFolder)
{
    std::vector<std::filesystem::path> files = JsonParser::searchFiles(pathToFolder);
    for (auto entry : files)
    {
        boost::json::object &obj = JsonParser::getValueFromFile(entry);
        CreatureParametersSPtr csptr= parseCreature(obj);
        FactoryMap_.insert(std::pair<std::string, CreatureParametersSPtr>(csptr->type_,csptr));
    }
}

CreatureParametersSPtr CreatureFactory::parseCreature(boost::json::object obj)
{
    return std::make_shared<CreatureParameters>(std::string(obj["type"].as_string().c_str()), 
                                                (float) obj["energy"].as_double(), 
                                                (float) obj["weight"].as_double(),
                                                (float) obj["hue"].as_double(),
                                                (float) obj["positionX"].as_double(),
                                                (float) obj["positionY"].as_double(),
                                                (float) obj["positionY"].as_double()
                                                );
}

CreatureFactory &CreatureFactory::getInstance()
{
    static CreatureFactory instance;
    return instance;
}

CreatureFactory::CreatureFactory()
{
    registerFolder(DEFAULT_CREATURE_PATH);
}