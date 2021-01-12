/**
 * \file
 * \author Bartłomiej Janowski
 */

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

namespace zpr
{
    /**
     * \author Bartłomiej Janowski
     * \class PyWebserver
     * The Python webserver, using SimpleHTTPServer which provides webview with the HTML data
     */
    class PyWebserver
    {
        /**
         * Path to CreatureSIM executable, relatively to which the HTML data is found
         */
        std::string executablePath = boost::dll::program_location().parent_path().string();

        /**
         * ThreadID of the server, neeeded to call an exception so that it terminates.
         */
        unsigned long threadId_;

        PyWebserver(const PyWebserver &) = delete;
        PyWebserver(PyWebserver &&) = delete;
        PyWebserver &operator=(PyWebserver &&) = delete;
        PyWebserver &operator=(const PyWebserver &) = delete;

    public:
        PyWebserver() = default;
        /**
         * Method for the server thread - gets the ThreadID and then runs the server until exception raised.
         */
        void run()
        {
            Py_Initialize();
            PyObject *py_main = PyImport_AddModule("__main__");
            PyRun_SimpleString(zpr_consts::PYTHON_THREADID.c_str());
            PyObject *func = PyObject_GetAttrString(py_main, "getThreadId");
            PyObject *args = PyTuple_Pack(0);
            PyObject *myResult = PyObject_CallObject(func, args);
            threadId_ = PyLong_AsUnsignedLong(myResult);
            PyModule_AddStringConstant(py_main, "executablePath", executablePath.c_str());
            PyRun_SimpleString(zpr_consts::PYTHON_SERVER.c_str());
        }
        /**
         * Method for the server termination - raises exceptions which should stop the server.
         */
        void terminate()
        {
            auto terminatingEx = PyExc_Exception;
            PyGILState_STATE gstate;
            gstate = PyGILState_Ensure();
            /**
             * This is necessary as on Linux two Exceptions are needed for SimpleHTTPServer to exit.
             */
            PyThreadState_SetAsyncExc(threadId_, terminatingEx);
            PyThreadState_SetAsyncExc(threadId_, terminatingEx);
            PyGILState_Release(gstate);
        }
    };
}; // namespace zpr