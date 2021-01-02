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
BOOST_AUTO_TEST_SUITE_END()