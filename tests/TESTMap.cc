/////////////////////////////////////////////////////////
///     Author: Magdalena Majkowska                   ///
///     Unit tests for Map class                      ///
/////////////////////////////////////////////////////////

#include <boost/test/unit_test.hpp>
#include "map.h"
namespace utf = boost::unit_test;
BOOST_AUTO_TEST_SUITE(MapSuite)
BOOST_AUTO_TEST_CASE(Convert2HSVTest,* utf::tolerance(0.01))
{
    Map map;
    Map::RGB black(0,0,0);
    Map::RGB white (255,255,255);
    Map::RGB red(255,0,0);
    Map::RGB lime(0,255,0);
    Map::RGB blue(0,0,255);
    Map::RGB olive(128,128,0);
    Map::RGB purple(128,0,128);
    Map::HSV hsvBlack= map.convert2HSV(black);
    Map::HSV hsvWhite = map.convert2HSV(white);
    Map::HSV hsvRed = map.convert2HSV(red);
    Map::HSV hsvLime = map.convert2HSV(lime);
    Map::HSV hsvBlue = map.convert2HSV(blue);
    Map::HSV hsvOlive = map.convert2HSV(olive);
    Map::HSV hsvPurple = map.convert2HSV(purple);
    //BLACK
    BOOST_CHECK( hsvBlack.h_==0.0f);
    BOOST_CHECK( hsvBlack.s_==0.0f);
    BOOST_CHECK( hsvBlack.v_==0.0f);
    //WHITE
    BOOST_CHECK( hsvWhite.h_==0.0f);
    BOOST_CHECK( hsvWhite.s_==0.0f);
    BOOST_CHECK( hsvWhite.v_==1.0f);
    //RED
    BOOST_CHECK( hsvRed.h_==0.0f);
    BOOST_CHECK( hsvRed.s_==1.0f);
    BOOST_CHECK( hsvRed.v_==1.0f);
    //LIME
    BOOST_CHECK( hsvLime.h_==120.0f);
    BOOST_CHECK( hsvLime.s_==1.0f);
    BOOST_CHECK( hsvLime.v_==1.0f);
    //BLUE
    BOOST_CHECK( hsvBlue.h_==240.0f);
    BOOST_CHECK( hsvBlue.s_==1.0f);
    BOOST_CHECK( hsvBlue.v_==1.0f);
    //OLIVE
    BOOST_CHECK( hsvOlive.h_==60.0f);
    BOOST_CHECK( hsvOlive.s_==1.0f);
    BOOST_CHECK( hsvOlive.v_==0.5f);
    //PURPLE
    BOOST_CHECK( hsvPurple.h_==300.0f);
    BOOST_CHECK( hsvPurple.s_==1.0f);
    BOOST_CHECK( hsvPurple.v_==0.5f);
}
BOOST_AUTO_TEST_CASE(Convert2RGBTest)
{
    Map map;
    Map::HSV hsvBlack(0.0f,0.0f,0.0f);
    Map::HSV hsvWhite(0.0f,0.0,1.0f);
    Map::HSV hsvRed(0.0f,1.0f,1.0f);
    Map::HSV hsvLime(120.0,1.0,1.0);
    Map::HSV hsvBlue(240.0f, 1.0f, 1.0f);
    Map::HSV hsvOlive(60.0f, 1.0f, 0.5f);
    Map::HSV hsvPurple(300.0f,1.0,0.5f);
    Map::RGB black = map.convert2RGB(hsvBlack);
    Map::RGB white= map.convert2RGB(hsvWhite);
    Map::RGB red = map.convert2RGB(hsvRed);
    Map::RGB lime= map.convert2RGB(hsvLime);
    Map::RGB blue= map.convert2RGB(hsvBlue);
    Map::RGB olive = map.convert2RGB(hsvOlive);
    Map::RGB purple = map.convert2RGB(hsvPurple);

    //BLACK
    BOOST_CHECK( black.r_==0.0f);
    BOOST_CHECK( black.g_==0.0f);
    BOOST_CHECK( black.b_==0.0f);
    //WHITE
    BOOST_CHECK( white.r_==255.0f);
    BOOST_CHECK( white.g_==255.0f);
    BOOST_CHECK( white.b_==255.0f);
    //RED
    BOOST_CHECK( red.r_==255.0f);
    BOOST_CHECK( red.g_==0.0f);
    BOOST_CHECK( red.b_==0.0f);
    //LIME
    BOOST_CHECK( lime.r_==0.0f);
    BOOST_CHECK( lime.g_==255.0f);
    BOOST_CHECK( lime.b_==0.0f);
    //BLUE
    BOOST_CHECK( blue.r_==0.0f);
    BOOST_CHECK( blue.g_==0.0f);
    BOOST_CHECK( blue.b_==255.0f);
    //OLIVE
    BOOST_CHECK( olive.r_==128.0f);
    BOOST_CHECK( olive.g_==128.0f);
    BOOST_CHECK( olive.b_==0.0f);
    //PURPLE
    BOOST_CHECK( purple.r_==128.0f);
    BOOST_CHECK( purple.g_==0.0f);
    BOOST_CHECK( purple.b_==128.0f);
}
BOOST_AUTO_TEST_SUITE_END()