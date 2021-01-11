/////////////////////////////////////////////////////////
///                                                   ///
///     Author: Bartlomiej Janowski                   ///
///     This class represents the Python webserver    ///
///     that is being used for the webview content    ///
///     delivery.                                     ///
///                                                   ///
/////////////////////////////////////////////////////////
#pragma once

#pragma warning (push, 0)

#include <thread>
#include <iostream>
#include <chrono>

#include <Python.h>

#include <boost/dll/runtime_symbol_info.hpp>

#pragma warning (pop)

#include "Flags.h"

class PyWebserver
{

    std::string executablePath = boost::dll::program_location().parent_path().string();

    std::thread pyServerThread_;

    unsigned long threadId_;

    void serve()
    {
        Py_Initialize();
        PyObject *py_main = PyImport_AddModule("__main__");
        PyRun_SimpleString (zpr_consts::PYTHON_THREADID.c_str());
        PyObject *func = PyObject_GetAttrString(py_main, "getThreadId");
        PyObject *args = PyTuple_Pack(0);
        PyObject* myResult = PyObject_CallObject(func, args);
        threadId_ = PyLong_AsUnsignedLong(myResult);
        PyModule_AddStringConstant(py_main, "executablePath", executablePath.c_str());
        PyRun_SimpleString(zpr_consts::PYTHON_SERVER.c_str());
    }

    PyWebserver(const PyWebserver &) = delete;
    PyWebserver(PyWebserver &&) = delete;
    PyWebserver &operator=(PyWebserver &&) = delete;
    PyWebserver &operator=(const PyWebserver &) = delete;

public:
    void run()
    {
        pyServerThread_ = std::thread(&PyWebserver::serve, this);
    }
    void terminate(){
        auto terminatingEx = PyExc_Exception;
        PyGILState_Ensure();
        PyThreadState_SetAsyncExc(threadId_, terminatingEx);
    }

    void join(){
        pyServerThread_.join();
    }
};
