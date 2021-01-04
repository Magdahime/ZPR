#include "Program.h"
#include <iostream>

#include "Python.h"

#include "boost/dll/runtime_symbol_info.hpp"

using namespace std;

void pyServ(const string& executablePath){
    Py_Initialize();
    auto py_main = PyImport_AddModule("__main__");
    PyModule_AddStringConstant(py_main, "executablePath", executablePath.c_str());
    PyRun_SimpleString(R"(
import http.server
import socketserver

PORT = 2137

DIRECTORY = executablePath + "/../resources/html"

class Handler(http.server.SimpleHTTPRequestHandler):
    def __init__(self, *args, **kwargs):
        super().__init__(*args, directory=DIRECTORY, **kwargs)

with socketserver.TCPServer(( "127.0.0.1" , PORT), Handler) as httpd:
    httpd.serve_forever()

)");
}

int main()
{
    string executablePath = boost::dll::program_location().parent_path().string();

    std::thread a(pyServ, executablePath);
    Program program;
    program.run();
    return 0;
}
