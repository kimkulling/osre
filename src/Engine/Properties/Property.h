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
#pragma once

#include "Properties/PropertiesCommon.h"
#include <cppcore/Common/Variant.h>

namespace OSRE {
namespace Properties {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class implements a simple property.
///	a property can be used to describe an arbitrary value which is supported by the 
///	ZFXCE2-environment. It has a value and an assigned name.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Property {
public:
    static const ui32 NotSet = 999999999;

public:
    Property( ui32 id, const String &name, const cppcore::Variant &value );
    ~Property() = default;
    void setPropertyName( const String &rName );
    const String &getPropertyName() const;
    void set( ui32 id, const String &rName, const cppcore::Variant &rValue );
    void setPropertyId( ui32 id );
    ui32 getPropertyId() const;
    const cppcore::Variant &getValue( ) const;
    bool operator == (const Property &rhs) const;

    // No copying
    Property( const Property &rhs ) = delete;
    Property &operator = ( const Property &rhs ) = delete;

private:
    ui32 m_Id;
    String m_Name;
    cppcore::Variant m_Value;
};

} // Namespace Properties
} // Namespace OSRE
