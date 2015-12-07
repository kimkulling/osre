/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
///	@class		::OSRE::Properties::IPropertyMap
///	@ingroup	Engine
///
///	@brief 
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractPropertyMap {
public:
    virtual ~AbstractPropertyMap();
    virtual void setProperty( ui32 id, const String &rName, const CPPCore::Variant &value ) = 0;
    virtual void setProperty( ui32 id, Property *Property ) = 0;
    virtual bool hasProperty( ui32 id ) const = 0;
    virtual Property *getProperty( ui32 id ) const = 0;
    virtual bool removeProperty( ui32 id ) = 0;
    virtual ui32 size() const = 0;
    virtual bool isEmpty() const = 0;
    virtual void clear() = 0;

protected:
    AbstractPropertyMap();
};

//-------------------------------------------------------------------------------------------------
inline 
AbstractPropertyMap::AbstractPropertyMap() {
    // empty
}

//-------------------------------------------------------------------------------------------------
inline 
AbstractPropertyMap::~AbstractPropertyMap() {
    // empty
}

//-------------------------------------------------------------------------------------------------

} // Namespace Properties
} // Namespace OSRE
