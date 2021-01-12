/////////////////////////////////////////////////////////
///                                                   ///
///     Author: Bartlomiej Janowski                   ///
///     This class represents the Python webserver    ///
///     that is being used for the webview content    ///
///     delivery.                                     ///
///                                                   ///
/////////////////////////////////////////////////////////
#pragma once

#ifndef LINUX_PRAGMA
#pragma warning(push, 0)
#endif //LINUX_PRAGMA

#include <thread>
#include <iostream>
#include <chrono>

#include <Python.h>

#include <boost/dll/runtime_symbol_info.hpp>

#ifndef LINUX_PRAGMA
#pragma warning(pop)
#endif //LINUX_PRAGMA

#include "Flags.h"

class PyWebserver
{

    std::string executablePath = boost::dll::program_location().parent_path().string();

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
    PyWebserver() = default;
    void run()
    {
        serve();
    }
    void terminate(){
        auto terminatingEx = PyExc_Exception;
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();
        /**
         * This is necessary as on Linux two Exceptions are needed for SimpleHTTPWebserver to exit.
         */
        PyThreadState_SetAsyncExc(threadId_, terminatingEx);
        PyThreadState_SetAsyncExc(threadId_, terminatingEx);
        PyGILState_Release(gstate);
    }
};
