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
#pragma once

#include <osre/Common/osre_common.h>

#include <cppcore/Common/Variant.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {
namespace Common {

/// @brief Struct to describe one environment variable. 
/// Variables are globally defined.
struct EnvVar {
    enum class Type {
        Int = 0,
        Str,
        None,
        NumTypes,
        InvalidType
    };

    String m_name;              ///< The variable name.
    CPPCore::Variant m_value;   ///< The variable value.

    /// @brief The class constructor for an integer value.
    /// @param[in] key      The key name
    /// @param[in] value    The integer value.
    EnvVar(const String &key, int value);

    /// @brief The class constructor for a string value.
    /// @param key 
    /// @param value 
    EnvVar(const String &key, const String &value);
    
    ///	@brief  The class destructor.
    ~EnvVar();

    /// @brief Will return the value type.
    /// @return The type.
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

inline EnvVar::~EnvVar() {
    // empty
}

inline EnvVar::Type EnvVar::getType() const {
    ::CPPCore::Variant::Type t = m_value.getType();
    switch (t) {
        case ::CPPCore::Variant::Int:
            return EnvVar::Type::Int;
        case ::CPPCore::Variant::String:
            return EnvVar::Type::Str;
        default:
            break;
    }
    return EnvVar::Type::None;
}

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief	This class is used to define environment variables in an application context. It will 
/// be available within your app-context.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Environment {
public:
    /// @brief  The default class constructor.
    Environment();

    ///	@brief  The class destructor.
    ~Environment();

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

    // No copying
    Environment(const Environment &) = delete;
    Environment &operator=(const Environment &) = delete;

private:
    using EnvVariables = ::CPPCore::THashMap<ui32, EnvVar *>;
    EnvVariables mEnvVariables;
    CPPCore::TArray<EnvVar *> mVars;
};

} // Namespace Common
} // Namespace OSRE
