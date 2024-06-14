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
#include "Common/Object.h"
#include "Common/StringUtils.h"

namespace OSRE {
namespace Common {

Object::Object(const String &objName) :
        mObjectName(objName, StringUtils::hashName(objName)), 
        mId(0),
        mRefcount(1) {
    // empty
}

void Object::get() {
    ++mRefcount;
}

void Object::release() {
    if( mRefcount > 0 ) {
        --mRefcount;
        if( 0 == mRefcount ) {
            delete this;
        }
    }
}

ui32 Object::getNumRefs() const {
    return mRefcount;
}

void Object::setName( const String &objName ) {
    if( mObjectName.RawString != objName ) {
        mObjectName.RawString = objName;
        mObjectName.Id = StringUtils::hashName(objName);
    }
}

const String &Object::getName() const {
    return mObjectName.RawString;
}

void Object::setGuid(guid id) {
    mId = id;
}

guid Object::getGuid() const {
    return mId;
}

} // Namespace Common
} // Namespace OSRE

