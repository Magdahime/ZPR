/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Unit tests for CreatureFactory class          ///
/////////////////////////////////////////////////////////
#include <boost/test/unit_test.hpp>
#include "CreatureFactory.h"

BOOST_AUTO_TEST_SUITE(CreatureFactorySuite)
BOOST_AUTO_TEST_CASE(constructorTest)
{
    BOOST_CHECK(CreatureFactory::getInstance().getFactoryMap().size() == 2);
}
BOOST_AUTO_TEST_CASE(create)
{
    CreatureParametersSPtr csptr1 = CreatureFactory::getInstance().createCreature("booba");
    CreatureParametersSPtr csptr2 = CreatureFactory::getInstance().createCreature("default");
    CreatureParametersSPtr csptr3 = CreatureFactory::getInstance().createCreature("userDefinedExample");
    BOOST_CHECK(csptr1-> type_ == "default");
    BOOST_CHECK(csptr2-> type_ == "default");
    BOOST_CHECK(csptr3-> type_ == "userDefinedExample");
}
BOOST_AUTO_TEST_SUITE_END()