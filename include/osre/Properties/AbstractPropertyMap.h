/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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

namespace CPPCore {
    class Variant;
}

namespace OSRE {
namespace Properties {

class Property;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  The abstract base class for a property map. Property maps can be used to manage global 
/// settings like resolution, the name of the application or the root media directory.
/// Derivate classes from this if you want to implement your own setting property map.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractPropertyMap {
public:
    /// @brief  The class destructor. 
    virtual ~AbstractPropertyMap();

    /// @brief
    /// @param
    /// @param
    /// @param
    virtual void setProperty( ui32 id, const String &rName, const CPPCore::Variant &value ) = 0;

    /// @brief
    /// @param
    /// @param
    virtual void setProperty( ui32 id, Property *Property ) = 0;

    /// @brief
    /// @param
    virtual bool hasProperty( ui32 id ) const = 0;

    /// @brief
    /// @param
    /// @return
    virtual Property *getProperty( ui32 id ) const = 0;

    /// @brief
    /// @param
    /// @return
    virtual bool removeProperty( ui32 id ) = 0;

    /// @brief
    /// @return
    virtual ui32 size() const = 0;

    /// @brief
    /// @return
    virtual bool isEmpty() const = 0;

    /// @brief
    virtual void clear() = 0;

protected:
    /// @brief
    AbstractPropertyMap();
};

inline 
AbstractPropertyMap::AbstractPropertyMap() {
    // empty
}

inline 
AbstractPropertyMap::~AbstractPropertyMap() {
    // empty
}

} // Namespace Properties
} // Namespace OSRE
