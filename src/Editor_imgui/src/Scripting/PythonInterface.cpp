/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Actions/ImportAction.h"

#include <osre/Common/Logger.h>
#include <osre/App/Stage.h>
#include <osre/App/World.h>
#include <osre/App/App.h>
#include <osre/app/Project.h>

#include <Python.h>
#include <structmember.h> // defines a python class in C++

namespace OSRE {
namespace Editor {

static constexpr c8 Tag[] = "PythonInterface";

using namespace ::OSRE::Common;
using namespace ::OSRE::App;

typedef struct Osre_World {
    PyObject_HEAD
    World *mWorld;
} Osre_World;

typedef struct Osre_Project {
    Stage *mStage;
    Project *mProject;
} Osre_Project;
        
Osre_Project *gActiceProject = nullptr;
PyTypeObject Osre_project_Type;

PyDoc_STRVAR(osre_project_new_doc, "ToDo.");
static PyObject *osre_project_new(PyObject*, PyObject *args, PyObject *keywds) {
    static char *kwlist[] = { "project_name", NULL };
    char *projectName = nullptr;
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", kwlist, &projectName)) {
        return nullptr;
    }

    gActiceProject = PyObject_New(Osre_Project, &Osre_project_Type);
    if (gActiceProject == nullptr) {
        return nullptr;
    }

    gActiceProject->mProject = new Project();
    
    gActiceProject->mProject->setProjectName(projectName);

    Py_RETURN_NONE;
}

PyDoc_STRVAR(osre_project_load_doc, "ToDo.");
static PyObject *osre_project_load(PyObject*, PyObject *args, PyObject *keywds) {
    static char *kwlist[] = { "project_file", NULL };
    char *project_file = nullptr;
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", kwlist, &project_file)) {
        return nullptr;
    }
    
    if (gActiceProject == nullptr) {
        gActiceProject = PyObject_New(Osre_Project, &Osre_project_Type);
        if (gActiceProject == nullptr) {
            return nullptr;
        }
    }

    gActiceProject->mProject = new Project();
    if (!gActiceProject->mProject->load(project_file, gActiceProject->mStage)) {
        osre_error(Tag, "Error while loading project file " + String(project_file));
    }

    Py_RETURN_NONE;
}

PyDoc_STRVAR(osre_project_save_doc, "ToDo.");
static PyObject *osre_project_save(PyObject*, PyObject *args, PyObject *keywds) {
    static char *kwlist[] = { "project_file", NULL };
    char *project_file = nullptr;
    if (!PyArg_ParseTupleAndKeywords(args, keywds, "s", kwlist, &project_file)) {
        return nullptr;
    }
    if (gActiceProject == nullptr) {
        return nullptr;
    }

    if (!gActiceProject->mProject->save(project_file, gActiceProject->mStage)) {
        osre_error(Tag, "Error while saving project file " + String(project_file));
    }

    Py_RETURN_NONE;
}

PyDoc_STRVAR(osre_project_close_doc, "ToDo.");
static PyObject *osre_project_close(PyObject*, PyObject*, PyObject*) {
    if (gActiceProject==nullptr) {
        return nullptr;
    }
    if (gActiceProject->mProject) {
        delete gActiceProject->mProject;
        gActiceProject->mProject = nullptr;
    }

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
static PyObject *osre_io_import(PyObject*, PyObject *args, PyObject *keywds) {
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
    PyModuleDef_HEAD_INIT,         /* init */
    "osre.io",                     /* name of module */
    NULL,                          /* module documentation, may be NULL */
    -1,                            /* size of per-interpreter state of the module, or -1 if the module keeps state in global variables. */
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

void PythonInterface::addPath(const String &path) {
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

    if (src.empty()) {
        osre_error(Tag, "Source is empty, cannot execute python script.")
        return false;
    }

    PyRun_SimpleString(src.c_str());

    return true;
}

} // namespace Editor
} // namespace OSRE
