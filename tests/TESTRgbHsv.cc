/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Unit tests for RGBHSV converter functions     ///
///     class                                         ///
/////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>
#include "structs/RgbHsv.h"
namespace utf = boost::unit_test;
BOOST_AUTO_TEST_SUITE(RgbHsvSuite)
BOOST_AUTO_TEST_CASE(Convert2HSVTest, *utf::tolerance(0.01))
{
    float epsilon = 2.f/255.f;
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
    BOOST_CHECK(fabs(hsvBlack.h_ - 0.0f) < epsilon);
    BOOST_CHECK(fabs(hsvBlack.s_ - 0.0f) < epsilon);
    BOOST_CHECK(fabs(hsvBlack.v_ - 0.0f) < epsilon);
    //WHITE
    BOOST_CHECK(fabs(hsvWhite.h_ - 0.0f) < epsilon);
    BOOST_CHECK(fabs(hsvWhite.s_ - 0.0f) < epsilon);
    BOOST_CHECK(fabs(hsvWhite.v_ - 1.0f) < epsilon);
    //RED
    BOOST_CHECK(fabs(hsvRed.h_ - 0.0f) < epsilon);
    BOOST_CHECK(fabs(hsvRed.s_ - 1.0f) < epsilon);
    BOOST_CHECK(fabs(hsvRed.v_ - 1.0f) < epsilon);
    //LIME
    BOOST_CHECK(fabs(hsvLime.h_ - 120.0f) < epsilon);
    BOOST_CHECK(fabs(hsvLime.s_ - 1.0f) < epsilon);
    BOOST_CHECK(fabs(hsvLime.v_ - 1.0f) < epsilon);
    //BLUE
    BOOST_CHECK(fabs(hsvBlue.h_ - 240.0f) < epsilon);
    BOOST_CHECK(fabs(hsvBlue.s_ - 1.0f) < epsilon);
    BOOST_CHECK(fabs(hsvBlue.v_ - 1.0f) < epsilon);
    //OLIVE
    BOOST_CHECK(fabs(hsvOlive.h_ - 60.0f) < epsilon);
    BOOST_CHECK(fabs(hsvOlive.s_ - 1.0f) < epsilon);
    BOOST_CHECK(fabs(hsvOlive.v_ - 0.502f) < epsilon);
    //PURPLE
    BOOST_CHECK(fabs(hsvPurple.h_ - 300.0f) < epsilon);
    BOOST_CHECK(fabs(hsvPurple.s_ - 1.0f) < epsilon);
    BOOST_CHECK(fabs(hsvPurple.v_ - 0.502f) < epsilon);
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