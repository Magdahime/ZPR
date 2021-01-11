/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Unit tests for Map class                      ///
/////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>
#include "structs/RgbHsv.h"
namespace utf = boost::unit_test;
BOOST_AUTO_TEST_SUITE(RgbHsvSuite)
BOOST_AUTO_TEST_CASE(Convert2HSVTest, *utf::tolerance(0.01))
{
    RGBvals black(0, 0, 0);
    RGBvals white(255, 255, 255);
    RGBvals red(255, 0, 0);
    RGBvals lime(0, 255, 0);
    RGBvals blue(0, 0, 255);
    RGBvals olive(128, 128, 0);
    RGBvals purple(128, 0, 128);
    HSVvals hsvBlack = convert2HSV(black);
    HSVvals hsvWhite = convert2HSV(white);
    HSVvals hsvRed = convert2HSV(red);
    HSVvals hsvLime = convert2HSV(lime);
    HSVvals hsvBlue = convert2HSV(blue);
    HSVvals hsvOlive = convert2HSV(olive);
    HSVvals hsvPurple = convert2HSV(purple);
    //BLACK
    BOOST_CHECK(hsvBlack.h_ == 0.0f);
    BOOST_CHECK(hsvBlack.s_ == 0.0f);
    BOOST_CHECK(hsvBlack.v_ == 0.0f);
    //WHITE
    BOOST_CHECK(hsvWhite.h_ == 0.0f);
    BOOST_CHECK(hsvWhite.s_ == 0.0f);
    BOOST_CHECK(hsvWhite.v_ == 1.0f);
    //RED
    BOOST_CHECK(hsvRed.h_ == 0.0f);
    BOOST_CHECK(hsvRed.s_ == 1.0f);
    BOOST_CHECK(hsvRed.v_ == 1.0f);
    //LIME
    BOOST_CHECK(hsvLime.h_ == 120.0f);
    BOOST_CHECK(hsvLime.s_ == 1.0f);
    BOOST_CHECK(hsvLime.v_ == 1.0f);
    //BLUE
    BOOST_CHECK(hsvBlue.h_ == 240.0f);
    BOOST_CHECK(hsvBlue.s_ == 1.0f);
    BOOST_CHECK(hsvBlue.v_ == 1.0f);
    //OLIVE
    BOOST_CHECK(hsvOlive.h_ == 60.0f);
    BOOST_CHECK(hsvOlive.s_ == 1.0f);
    BOOST_CHECK(hsvOlive.v_ == 0.5f);
    //PURPLE
    BOOST_CHECK(hsvPurple.h_ == 300.0f);
    BOOST_CHECK(hsvPurple.s_ == 1.0f);
    BOOST_CHECK(hsvPurple.v_ == 0.5f);
}
BOOST_AUTO_TEST_CASE(Convert2RGBTest)
{
    HSVvals hsvBlack(0.0f, 0.0f, 0.0f);
    HSVvals hsvWhite(0.0f, 0.0, 1.0f);
    HSVvals hsvRed(0.0f, 1.0f, 1.0f);
    HSVvals hsvLime(120.0, 1.0, 1.0);
    HSVvals hsvBlue(240.0f, 1.0f, 1.0f);
    HSVvals hsvOlive(60.0f, 1.0f, 0.5f);
    HSVvals hsvPurple(300.0f, 1.0, 0.5f);
    RGBvals black = convert2RGB(hsvBlack);
    RGBvals white = convert2RGB(hsvWhite);
    RGBvals red = convert2RGB(hsvRed);
    RGBvals lime = convert2RGB(hsvLime);
    RGBvals blue = convert2RGB(hsvBlue);
    RGBvals olive = convert2RGB(hsvOlive);
    RGBvals purple = convert2RGB(hsvPurple);

    //BLACK
    BOOST_CHECK(black.r_ == 0.0f);
    BOOST_CHECK(black.g_ == 0.0f);
    BOOST_CHECK(black.b_ == 0.0f);
    //WHITE
    BOOST_CHECK(white.r_ == 255.0f);
    BOOST_CHECK(white.g_ == 255.0f);
    BOOST_CHECK(white.b_ == 255.0f);
    //RED
    BOOST_CHECK(red.r_ == 255.0f);
    BOOST_CHECK(red.g_ == 0.0f);
    BOOST_CHECK(red.b_ == 0.0f);
    //LIME
    BOOST_CHECK(lime.r_ == 0.0f);
    BOOST_CHECK(lime.g_ == 255.0f);
    BOOST_CHECK(lime.b_ == 0.0f);
    //BLUE
    BOOST_CHECK(blue.r_ == 0.0f);
    BOOST_CHECK(blue.g_ == 0.0f);
    BOOST_CHECK(blue.b_ == 255.0f);
    //OLIVE
    BOOST_CHECK(olive.r_ == 128.0f);
    BOOST_CHECK(olive.g_ == 128.0f);
    BOOST_CHECK(olive.b_ == 0.0f);
    //PURPLE
    BOOST_CHECK(purple.r_ == 128.0f);
    BOOST_CHECK(purple.g_ == 0.0f);
    BOOST_CHECK(purple.b_ == 128.0f);
}
BOOST_AUTO_TEST_SUITE_END()