/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) Kim Kulling

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
#include <osre/Common/Object.h>

namespace OSRE {
namespace Common {

//-------------------------------------------------------------------------------------------------
Object::Object( const String &objectName )
: m_ObjectName( objectName )
, m_Refcount( 1 ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
Object::~Object() {
    // empty
}

//-------------------------------------------------------------------------------------------------
void Object::get() {
    ++m_Refcount;
}

//-------------------------------------------------------------------------------------------------
void Object::release() {
    if( m_Refcount > 0 ) {
        --m_Refcount;
        if( 0 == m_Refcount ) {
            delete this;
        }
    }
}

//-------------------------------------------------------------------------------------------------
ui32 Object::getNumRefs() const {
    return m_Refcount;
}

//-------------------------------------------------------------------------------------------------
void Object::setName( const String &objName ) {
    if( m_ObjectName != objName ) {
        m_ObjectName = objName;
    }
}

//-------------------------------------------------------------------------------------------------
const String &Object::getName() const {
    return m_ObjectName;
}

//-------------------------------------------------------------------------------------------------

} // Namespace Common
} // Namespace OSRE

