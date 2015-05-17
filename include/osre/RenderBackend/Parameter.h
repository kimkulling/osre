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

#include <osre/Common/Types.h>

namespace OSRE {
namespace RenderBackend {

enum ParameterType {
    PT_None,
    PT_Int,
    PT_Float,
    PT_Float2,
    PT_Float3,
    PT_Mat4, 
    PT_Mat4Array
};

struct OSRE_EXPORT ParamDataBlob {
    void *m_data;
    ui32  m_size;

    void *getData() const;
    static ParamDataBlob *create( ParameterType type, ui32 arraySize );
};

struct OSRE_EXPORT Parameter {
    String     m_name;
    ParameterType m_type;
    ui32          m_numItems;
    ParamDataBlob m_data;
    Parameter    *m_next;

    Parameter();
    ~Parameter();

    static ui32 getParamDataSize( ParameterType type, ui32 arraySize );
    static Parameter *create( const String &name, ParameterType type, ui32 arraySize=1 );
};

} // Namespace RenderBackend
} // Namespace OSRE
