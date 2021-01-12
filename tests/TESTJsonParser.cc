/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Unit tests for JSONPARSER class               ///
/////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>
#include "JsonParser.h"
#include "CreatureFactory.h"
namespace utf = boost::unit_test;

using namespace zpr;

BOOST_AUTO_TEST_SUITE(JsonParserSuite)
BOOST_AUTO_TEST_CASE(searchFilesTest)
{
    BOOST_CHECK(JsonParser::searchFiles(CreatureFactory::getInstance().getConfigurationPath()).size() == 2);
}
BOOST_AUTO_TEST_CASE(getValueFromFileTest)
{
    std::vector<std::filesystem::path> files = JsonParser::searchFiles(CreatureFactory::getInstance().getConfigurationPath());
    for(auto entry : files){
        boost::json::object obj = JsonParser::getValueFromFile(entry);
        BOOST_CHECK(obj.size() == 7);
        BOOST_CHECK(obj["type"].is_string() == true);
        BOOST_CHECK(obj["energy"].is_double() == true);
        BOOST_CHECK(obj["weight"].is_double() == true);
        BOOST_CHECK(obj["hue"].is_double() == true);
        BOOST_CHECK(obj["positionX"].is_double() == true);
        BOOST_CHECK(obj["positionY"].is_double() == true);
        BOOST_CHECK(obj["speedMultiplier"].is_double() == true);
    }
}

BOOST_AUTO_TEST_CASE(saveJsonToFileTest)
{
    std::string filename= "test";
    for(int i =0; i<100; ++i){
        JsonParser::saveJsonToFile(filename, "test", CreatureFactory::getInstance().getConfigurationPath());
    }
    BOOST_CHECK(JsonParser::searchFiles(CreatureFactory::getInstance().getConfigurationPath()).size() == 102);
    for(int i =0; i<100; i++){
        std::filesystem::remove(CreatureFactory::getInstance().getConfigurationPath() + filename + std::to_string(i) + ".json");
    }
    std::filesystem::remove(CreatureFactory::getInstance().getConfigurationPath() + filename + ".json");
}
BOOST_AUTO_TEST_SUITE_END()