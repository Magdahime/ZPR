/////////////////////////////////////////////////////////
///                                                   ///
///     Author: Bartlomiej Janowski                   ///
///     Aim of this file is to organise any of        ///
///     the flags or general constants in the program ///
///                                                   ///
/////////////////////////////////////////////////////////
#pragma once
#include <string>

namespace zpr_dev_flags
{
    const bool WEBVIEW_DEBUG = true; // if webview allows access to Developer Menu
}
namespace zpr_paths
{
    const std::string HTTP_PATH = "http://127.0.0.1:2137/dashboard/index.html"; // path to opening webview
}

namespace zpr_windows
{
    const std::string SF_NAME = "CreatureSim";
    const std::string WV_NAME = "CreatureSim Webview";
    const int WV_X = 800;
    const int WV_Y = 600;
    const int SF_X = 800;
    const int SF_Y = 800;
} // namespace zpr_windows

namespace zpr_consts
{
    const int statistics_sleep_millis = 1000;
}