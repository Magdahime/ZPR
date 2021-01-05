/////////////////////////////////////////////////////////
///                                                   ///
///     Author: Bartlomiej Janowski                   ///
///     This class represents the Python webserver    ///
///     that is being used for the webview content    ///
///     delivery.                                     ///
///                                                   ///
/////////////////////////////////////////////////////////
#pragma once

#include "Python.h"

#include "boost/dll/runtime_symbol_info.hpp"

#include <thread>
#include <iostream>
#include <chrono>
#include "Flags.h"

class PyWebserver
{

    std::string executablePath = boost::dll::program_location().parent_path().string();

    std::thread pyServerThread_;

    int threadId_;

    void serve()
    {
        Py_Initialize();
        PyGILState_Ensure();
        PyObject *py_main = PyImport_AddModule("__main__");
        PyRun_SimpleString (zpr_consts::PYTHON_THREADID.c_str());
        PyObject *func = PyObject_GetAttrString(py_main, "getThreadId");
        PyObject *args = PyTuple_Pack(0);
        PyObject* myResult = PyObject_CallObject(func, args);
        threadId_ = _PyLong_AsInt(myResult);
        std::cout<<"\n\n\n\n\n\n"<<threadId_<<"\n\n\n\n\n\n";
        PyModule_AddStringConstant(py_main, "executablePath", executablePath.c_str());
        PyRun_SimpleString(zpr_consts::PYTHON_SERVER.c_str());
    }

public:
    void run()
    {
        pyServerThread_ = std::thread(&PyWebserver::serve, this);
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        // terminate();
    }
    void terminate(){
        auto i = PyExc_Exception;
        PyThreadState_SetAsyncExc(threadId_, i);
    }
};
