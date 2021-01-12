/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Unit tests for CreatureFactory class          ///
/////////////////////////////////////////////////////////
#include <boost/test/unit_test.hpp>
#include "CreatureFactory.h"
#include "JsonParser.h"

BOOST_AUTO_TEST_SUITE(CreatureFactorySuite)
BOOST_AUTO_TEST_CASE(constructorTest)
{
    BOOST_CHECK(CreatureFactory::getInstance().getFactoryMap().size() == 2);
}
BOOST_AUTO_TEST_CASE(createTest)
{
    CreatureParametersSPtr csptr1 = CreatureFactory::getInstance().createCreature("booba");
    CreatureParametersSPtr csptr2 = CreatureFactory::getInstance().createCreature("default");
    CreatureParametersSPtr csptr3 = CreatureFactory::getInstance().createCreature("userDefinedExample");
    BOOST_CHECK(csptr1-> type_ == "default");
    BOOST_CHECK(csptr2-> type_ == "default");
    BOOST_CHECK(csptr3-> type_ == "userDefinedExample");
}
BOOST_AUTO_TEST_CASE(parseCreatureTest)
{
    std::filesystem::path path = CreatureFactory::getInstance().getConfigurationPath() + "defaultCreatures.json";
    boost::json::object obj = JsonParser::getValueFromFile(path);
    CreatureParametersSPtr csptr = CreatureFactory::getInstance().parseCreature(obj);
    BOOST_CHECK(csptr-> energy_ == 50.0);
    BOOST_CHECK(csptr-> weight_ == 1000.0 );
    BOOST_CHECK(csptr-> hue_ == 118.0);
    BOOST_CHECK(csptr-> positionX_ == 110.0);
    BOOST_CHECK(csptr-> positionY_ == 110.0);
    BOOST_CHECK(csptr-> speedMultiplier_ == 1.0);
}
BOOST_AUTO_TEST_CASE(getParsedValuesTest)
{
    std::string filename = "parsedValues";
    std::string path = CreatureFactory::getInstance().getConfigurationPath() + filename + ".json";
    std::string parsed = CreatureFactory::getInstance().getParsedValues("default");
    JsonParser::saveJsonToFile(filename, parsed, CreatureFactory::getInstance().getConfigurationPath());
    boost::json::object obj = JsonParser::getValueFromFile(path);
    CreatureParametersSPtr csptr = CreatureFactory::getInstance().parseCreature(obj);
    CreatureParametersSPtr csptrTest = CreatureFactory::getInstance().getFactoryMap()["default"];
    BOOST_CHECK(csptr->type_ == csptrTest-> type_);
    BOOST_CHECK(csptr->energy_ == csptrTest-> energy_);
    BOOST_CHECK(csptr->weight_ == csptrTest-> weight_);
    BOOST_CHECK(csptr->hue_ == csptrTest-> hue_);
    BOOST_CHECK(csptr->positionX_ == csptrTest-> positionX_);
    BOOST_CHECK(csptr->positionY_ == csptrTest-> positionY_);
    BOOST_CHECK(csptr->speedMultiplier_ == csptrTest-> speedMultiplier_);
    std::filesystem::remove(path);

}
BOOST_AUTO_TEST_SUITE_END()