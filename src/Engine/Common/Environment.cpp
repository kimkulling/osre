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
#include <osre/Common/Environment.h>
#include <osre/Common/StringUtils.h>

namespace OSRE {
namespace Common {

EnvVar* Environment::findVar(const c8* varName) const {
    if (nullptr == varName) {
        return nullptr;
    }
    EnvVar* var(nullptr);
    HashId id = StringUtils::hashName(varName);
    if ( !mEnvVariables.hasKey( id ) ) {
        return nullptr;
    }
    if (!mEnvVariables.hasKey(id)) {
        return nullptr;
    }

    if (mEnvVariables.getValue(id, var)) {
        return var;
    }

    return nullptr;
}

void Environment::addIntVar(const c8* name, int value) {
    if (nullptr != findVar(name)) {
        return;
    }

    EnvVar* var = new EnvVar(name, value);
    mEnvVariables.insert(StringUtils::hashName(var->m_name.c_str()), var);
}

void Environment::addStrVar(const c8* name, const c8* value) {
    if (nullptr != findVar(name)) {
        return;
    }

    EnvVar* var = new EnvVar(name, value);
    mEnvVariables.insert(StringUtils::hashName(name), var );
}

void Environment::addVariable( EnvVar *var) {
    if (nullptr == findVar(var->m_name.c_str())) {
        return;
    }

    mEnvVariables.insert(StringUtils::hashName(var->m_name.c_str()), var);
}

} // Namespace Common
} // Namespace OSRE

