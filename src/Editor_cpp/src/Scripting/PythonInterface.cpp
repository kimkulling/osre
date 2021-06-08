#include "PythonInterface.h"

#include <Python.h>

namespace OSRE {
namespace Editor {

PythonInterface::PythonInterface() :
        mCreated(false) {
    // empty
}

PythonInterface::~PythonInterface() {
    // empty
}

bool PythonInterface::create() {
    if (mCreated) {
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
        return false;
    }
    if (Py_FinalizeEx() < 0) {
        exit(120);
    }

    mCreated = false;

    return true;
}

bool PythonInterface::runScript() {
    if (!mCreated) {
        return false;
    }
    PyRun_SimpleString("from time import time,ctime\n"
                       "print('Today is', ctime(time()))\n");

    return true;
}

} // Namespace Editor
} // namespace OSRE
