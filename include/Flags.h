/**
 * \file
 * 
 * \author Bartlomiej Janowski
 * 
 * Flags.h includes all constants used across the program, incl. flags changing application behavior.
 */

#pragma once

#pragma warning(push, 0)

#include <string>

#pragma warning(pop)

namespace zpr_dev_flags
{
    const char WEBVIEW_DEBUG = 1; // if webview allows access to Developer Menu
} // namespace zpr_dev_flags

namespace zpr_paths
{
    const std::string HTTP_ADDR = "127.0.0.1";
    const std::string HTTP_PORT = "2137";
    const std::string HTTP_PATH = "http://" + HTTP_ADDR + ":" + HTTP_PORT + "/dashboard/index.html"; // path to opening webview
} // namespace zpr_paths

namespace zpr_windows
{
    const std::string SF_NAME = "CreatureSim";
    const std::string WV_NAME = "CreatureSim Webview";
    const int WV_X = 1200;
    const int WV_Y = 800;
    const int SF_X = 800;
    const int SF_Y = 800;
} // namespace zpr_windows

namespace zpr_consts
{
    const int statistics_sleep_millis = 1000;
    const std::string PYTHON_SERVER =
        R"(
import http.server
import socketserver

PORT = )" +
        zpr_paths::HTTP_PORT +
        R"(

DIRECTORY = executablePath + "/../resources/html"

class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=DIRECTORY, **kwargs)

with socketserver.TCPServer(( ")" +
        zpr_paths::HTTP_ADDR +
        R"(" , PORT), Handler) as httpd:
    httpd.serve_forever()

)";
    const std::string PYTHON_THREADID =
        R"(
import threading
def getThreadId():
    return threading.get_ident()
)";
} // namespace zpr_consts