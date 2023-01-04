/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#pragma once

#include <osre/Common/osre_common.h>

#include <cppcore/Common/Variant.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {
namespace Common {

struct EnvVar {
    enum Type {
        Int,
        Str,
        None
    };

    String m_name;
    cppcore::Variant m_value;

    EnvVar(const String &key, int value);
    EnvVar(const String &key, const String &value);
    ~EnvVar() = default;
    Type getType() const;
};

inline EnvVar::EnvVar(const String &key, int value) :
        m_name(key), m_value() {
    m_value.setInt(value);
}

inline EnvVar::EnvVar(const String &key, const String &value) :
        m_name(key), m_value() {
    m_value.setStdString(value);
}

inline EnvVar::Type EnvVar::getType() const {
    ::cppcore::Variant::Type t = m_value.getType();
    switch (t) {
        case ::cppcore::Variant::Int:
            return EnvVar::Int;
        case ::cppcore::Variant::String:
            return EnvVar::Str;
        default:
            break;
    }
    return EnvVar::None;
}

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief	This class is used to define environment variables in an application context. It will 
/// be available within your app-context.
//-------------------------------------------------------------------------------------------------
class Environment {
public:
    /// @brief  The class constructor.
    Environment() = default;

    ///	@brief  The class destructor.
    ~Environment() = default;

    /// @brief  Will search for a variable by its name.
    /// @param[in] var     The variable name.
    /// @return The variable or nullptr.
    EnvVar *findVar(const c8 *var) const;

    /// @brief Will add a new int variable.
    /// @param[in] name     The variable name.
    /// @param[in] value    The variable instance.
    void addIntVar(const c8 *name, int value);

    /// @brief Will add a new string variable.
    /// @param[in] name     The variable name.
    /// @param[in] value    The variable instance.
    void addStrVar(const c8 *name, const c8 *value);
    
    /// @brief Will add a new generic variable.
    /// @param[in] name     The variable name.
    /// @param[in] value    The variable instance.
    void addVariable(EnvVar *var);

private:
    using EnvVariables = ::cppcore::THashMap<ui32, EnvVar *>;
    EnvVariables mEnvVariables;
};

} // Namespace Common
} // Namespace OSRE
