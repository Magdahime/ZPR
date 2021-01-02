/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Unit tests for JSONPARSER class               ///
/////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>
#include "JsonParser.h"
namespace utf = boost::unit_test;
BOOST_AUTO_TEST_SUITE(JsonParserSuite)
BOOST_AUTO_TEST_CASE(searchFiles)
{
    BOOST_CHECK(JsonParser::searchFiles(".\\config\\creature").size() == 2);
}
BOOST_AUTO_TEST_CASE(getValueFromFile)
{
    std::vector<std::filesystem::path> files = JsonParser::searchFiles(".\\config\\creature");
    for(auto entry : files){
        boost::json::object& obj = JsonParser::getValueFromFile(entry);
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
BOOST_AUTO_TEST_SUITE_END()