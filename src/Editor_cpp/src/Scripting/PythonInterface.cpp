/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2022 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#include "PythonInterface.h"
#include <osre/Common/Logger.h>
#include <osre/App/Stage.h>
#include <osre/App/App.h>

#include "Actions/ImportAction.h"

#include <Python.h>
#include <structmember.h> // defines a python class in C++

namespace OSRE {
namespace Editor {

static const c8 *Tag = "PythonInterface";

using namespace ::OSRE::App;

OsreContext *sCurrentContext = nullptr;

void setOsreContext( OsreContext *ctx ) {
    sCurrentContext = ctx;
}

PyDoc_STRVAR(osre_app_create_doc, "ToDo.");
static PyObject *osre_app_create(PyObject *self, PyObject *args, PyObject *keywds) {
    static char *kwlist[] = { "app_name", NULL };
    PyObject *appName = nullptr;
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", kwlist, &appName)) {
        return nullptr;
    }

    OsreApp *app = new OsreApp("txt");

    Py_RETURN_NONE;
}

PyDoc_STRVAR(osre_app_exit_doc, "ToDo.");
static PyObject *osre_app_exit(PyObject *self, PyObject *args, PyObject *keywds) {
    Py_RETURN_NONE;
}

static PyMethodDef osre_app_methods[] = {
    { "create", (PyCFunction)osre_app_create, METH_VARARGS, "ToDo!" },
    { "exit", (PyCFunction)osre_app_exit, METH_VARARGS, "ToDo!" },
    { NULL, NULL, 0, NULL } /* Sentinel */
};

static struct PyModuleDef osre_app_module = {
    PyModuleDef_HEAD_INIT,
    "osre.app", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1, /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    osre_app_methods
};

PyMODINIT_FUNC PyInit_osre_app() {
    return PyModule_Create(&osre_app_module);
}

PyDoc_STRVAR(osre_project_new_doc, "ToDo.");
static PyObject *osre_project_new(PyObject *self, PyObject *args, PyObject *keywds) {
    Py_RETURN_NONE;
}

PyDoc_STRVAR(osre_project_new_load, "ToDo.");
static PyObject *osre_project_load(PyObject *self, PyObject *args, PyObject *keywds) {
    Py_RETURN_NONE;
}

PyDoc_STRVAR(osre_project_new_save, "ToDo.");
static PyObject *osre_project_save(PyObject *self, PyObject *args, PyObject *keywds) {
    Py_RETURN_NONE;
}

PyDoc_STRVAR(osre_project_new_close, "ToDo.");
static PyObject *osre_project_close(PyObject *self, PyObject *args, PyObject *keywds) {
    Py_RETURN_NONE;
}

static PyMethodDef osre_project_methods[] = {
    { "new", (PyCFunction) osre_project_new, METH_VARARGS, "ToDo!" },
    { "load", (PyCFunction) osre_project_load, METH_VARARGS, "ToDo!" },
    { "save", (PyCFunction) osre_project_save, METH_VARARGS, "ToDo!" },
    { "close", (PyCFunction) osre_project_close, METH_VARARGS, "ToDo!" },
    { NULL, NULL, 0, NULL } /* Sentinel */
};

static struct PyModuleDef osre_project_module = {
    PyModuleDef_HEAD_INIT,
    "osre.project", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1, /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    osre_project_methods
};

PyMODINIT_FUNC PyInit_osre_project() {
    return PyModule_Create(&osre_project_module);
}

PyDoc_STRVAR(osre_io_import_doc, "ToDo.");
static PyObject *osre_io_import(PyObject *self, PyObject *args, PyObject *keywds) {
    int voltage;
    const char *state = "a stiff";
    const char *action = "voom";
    const char *type = "Norwegian Blue";

    static char *kwlist[] = { "voltage", "state", "action", "type", NULL };

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "i|sss", kwlist,
                &voltage, &state, &action, &type))
        return NULL;

    printf("-- This parrot wouldn't %s if you put %i Volts through it.\n",
            action, voltage);
    printf("-- Lovely plumage, the %s -- It's %s!\n", type, state);

    Py_RETURN_NONE;
}

static PyMethodDef osre_io_methods[] = {
    { "import", (PyCFunction) osre_io_import, METH_VARARGS, "Imports a new asset." },
    { NULL, NULL, 0, NULL } /* Sentinel */
};

static struct PyModuleDef osre_io_module = {
    PyModuleDef_HEAD_INIT,
    "osre.io", /* name of module */
    NULL, /* module documentation, may be NULL */
    -1, /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
    osre_io_methods
};

PyMODINIT_FUNC PyInit_osre_io() {
    return PyModule_Create(&osre_io_module);
}

PythonInterface::PythonInterface() :
        mCreated(false),
        mPaths(),
        mApp(nullptr) {
    // empty
}

bool PythonInterface::create(App::AppBase *app) {
    if (mCreated) {
        osre_error(Tag, "Error while create, Python interface is already created.")
        return false;
    }

    const wchar_t *program = L"osre_ed";
    Py_SetProgramName(program); /* optional but recommended */
    Py_Initialize();

    PyObject *mod = nullptr;
    mod = PyInit_osre_app();
    if (mod == nullptr) {
        osre_error(Tag, "Error while creating app-module.")
    }

    mod = PyInit_osre_project();
    if (mod == nullptr) {
        osre_error(Tag, "Error while creating project-module.")
    }
    mod = PyInit_osre_io();
    if (mod == nullptr) {
        osre_error(Tag, "Error while creating io-module.")
    }

    mApp = app;
    mCreated = true;

    return true;
}

bool PythonInterface::destroy() {
    if (!mCreated) {
        osre_error(Tag, "Error while destoy, Python interface is not created.")

        return false;
    }
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }

    mCreated = false;

    return true;
}

void PythonInterface::addPath( const String &path ) {
    if (path.empty()) {
        return;
    }
    mPaths.add(path);
}

bool PythonInterface::runScript(const String &src) {
    if (!mCreated) {
        osre_error(Tag, "Error while calling scrpt, Python interface is not created.")
        return false;
    }

    PyRun_SimpleString(src.c_str());

    return true;
}

} // namespace Editor
} // namespace OSRE
