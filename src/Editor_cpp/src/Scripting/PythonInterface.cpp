/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/App/World.h>

#include <Python.h>
#include <structmember.h> // defines a python class in C++

namespace OSRE {
namespace Editor {

static const c8 *Tag = "PythonInterface";

using namespace ::OSRE::App;

struct PyWorldObject {
    PyObject_HEAD;
    World *World;
};

static void PyWorld_dealloc(PyWorldObject *self) {
    delete self->World;
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *PyWorld_new(PyTypeObject *type, PyObject *args, PyObject *kwds) {
    PyWorldObject *self;
    self = (PyWorldObject*) type->tp_alloc(type, 0);
    return (PyObject*) self;
}

static int PyWorldObject_init(PyWorldObject *self, PyObject *args, PyObject *kwds) {
    if (nullptr == self) {
        return -1;
    }

    return 0; // success (of init)
}

static PyObject *PyWorldObject_repr(PyWorldObject *self) {
    const String &name = self->World->getName();
    return Py_BuildValue("s", name.c_str());
};

static int PyWorld_len(PyWorldObject *self) {
    return 1;
}

static PyObject *PyWorld_getitem(PyWorldObject *self, PyObject *args) {
    c8 *type = nullptr;
    if (!PyArg_ParseTuple(args, "s", &type)) {
        return nullptr;
    }

    if (0 == strncmp(type, "camera", sizeof("type"))) {
        //
    } else if (0 == strncmp(type, "entity", sizeof("entity")))  {
        //
    }
}
static int PyWorld_setitem(PyWorldObject *self, PyObject *ix, PyObject *val) {
    return 0;
}

static PyMappingMethods PyWorld_mappings = {
    (lenfunc)PyWorld_len,
    (binaryfunc)PyWorld_getitem,
    (objobjargproc)PyWorld_setitem
};

static PyMemberDef PyWorldObject_members[] = {
    //{ "capacity", T_PYSSIZET, offsetof(PyWorldObject, len), READONLY, "" },
    { NULL, 0, 0, 0, NULL }
};

static PyObject *create_world(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, "s")) {
        return nullptr;
    }

    return nullptr;
}

static PyObject *add_node(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, "s")) {
        return nullptr;
    }
}

static PyObject *get_Node(PyObject *self, PyObject *args) {
    if (!PyArg_ParseTuple(args, "s")) {
        return nullptr;
    }
}

static PyMethodDef PyWorld_methods[] = {
    { "create_world", (PyCFunction)create_world, METH_VARARGS, "Randomly change bits in the chromosone" },
    { NULL, NULL, 0, NULL }
};

static PyTypeObject WorldPyObject = {
    PyVarObject_HEAD_INIT(NULL, 0) 
    "osre_ed.World",                /* tp_name */
    sizeof(PyWorldObject), /* tp_basicsize */
    0,                              /* tp_itemsize */
    (destructor) PyWorld_dealloc, /* tp_dealloc */
    0, /* tp_print */
    0, /* tp_getattr */
    0, /* tp_setattr */
    0, /* tp_reserved */
    (reprfunc)PyWorldObject_repr, /* tp_repr */
    0, /* tp_as_number */
    0, /* tp_as_sequence */
    &PyWorld_mappings, /* tp_as_mapping */
    0, /* tp_hash */
    0, /* tp_call */
    0, /* tp_str */
    0, /* tp_getattro */
    0, /* tp_setattro */
    0, /* tp_as_buffer */
    Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE, /* tp_flags */
    "PyWorld objects", /* tp_doc */
    0, /* tp_traverse */
    0, /* tp_clear */
    0, /* tp_richcompare */
    0, /* tp_weaklistoffset */
    // (getiterfunc)Chromosone_getiter, /* tp_iter */
    PyObject_SelfIter, /* tp_iter */
    0, /* tp_iternext */
    PyWorld_methods, /* tp_methods */
    PyWorldObject_members, /* tp_members */
    0, /* tp_getset */
    0, /* tp_base */
    0, /* tp_dict */
    0, /* tp_descr_get */
    0, /* tp_descr_set */
    0, /* tp_dictoffset */
    (initproc)PyWorldObject_init, /* tp_init */
    0, /* tp_alloc */
    PyWorld_new /* tp_new */
};

PyMODINIT_FUNC PyInit_custom() {
    PyObject *m;
    if (PyType_Ready(&WorldPyObject) < 0)
        return NULL;

    return m;
}

PythonInterface::PythonInterface() :
        mCreated(false),
        mPaths() {
    // empty
}

PythonInterface::~PythonInterface() {
    // empty
}

bool PythonInterface::create() {
    if (mCreated) {
        osre_error(Tag, "Error while create, Python interface is already created.")
        return false;
    }

    const wchar_t *program = L"osre_ed";
    Py_SetProgramName(program); /* optional but recommended */
    Py_Initialize();

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
