/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Properties/Property.h>

#include <cppcore/Common/Variant.h>

namespace OSRE {
namespace Properties {

Property::Property( ui32 id, const String &name, const CPPCore::Variant &value ) 
: m_Id( id )
, m_Name( name )
, m_Value( value ) {
    // empty
}

Property::Property( const Property &rhs ) 
: m_Id( rhs.m_Id )
, m_Name( rhs.m_Name )
, m_Value( rhs.m_Value ) {
    // empty
}

Property::~Property() {
    // empty
}

void Property::setPropertyName( const String &name ) {
    m_Name = name;
}

const String &Property::getPropertyName() const {
    return m_Name;
}

void Property::set( ui32 id, const String &rName, const CPPCore::Variant &value ) {
    m_Id = id;
    m_Name = rName;
    m_Value = value;
}

void Property::setPropertyId(ui32 id) {
    m_Id = id;
}

ui32 Property::getPropertyId() const {
    return m_Id;
}

const CPPCore::Variant &Property::getValue( ) const {
    return m_Value;
}

Property &Property::operator = ( const Property &rhs ) {
    if ( rhs == *this ) {
        return *this;
    }

    m_Id = rhs.m_Id;
    m_Value = rhs.m_Value;

    return *this;
}

bool Property::operator == ( const Property &rhs ) const {
    return ( m_Id == rhs.m_Id && m_Value == rhs.m_Value );
}

} // Namespace Properties
} // Namespace OSRE
