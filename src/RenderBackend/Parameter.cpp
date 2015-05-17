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
#include <osre/RenderBackend/Parameter.h>

namespace OSRE {
namespace RenderBackend {

void *ParamDataBlob::getData() const {
    return m_data;
}

ParamDataBlob *ParamDataBlob::create( ParameterType type, ui32 arraySize ) {
    ParamDataBlob *blob = new ParamDataBlob;
    switch( type ) {
    case PT_Int:
        blob->m_size = sizeof( i32 );
        break;
    case PT_Float:
        blob->m_size = sizeof( f32 );
        break;
    case PT_Float2:
        blob->m_size = sizeof( f32 ) * 2;
        break;
    case PT_Float3:
        blob->m_size = sizeof( f32 ) * 3;
        break;
    case PT_Mat4:
        blob->m_size = sizeof( f32 ) * 16;
        break;
    case PT_Mat4Array:
        blob->m_size = sizeof( f32 ) * 16 * arraySize;
        break;
    default:
        blob->m_size = 0;
        break;
    }
    blob->m_data = new uc8[ blob->m_size ];
    ::memset( blob->m_data, 0, blob->m_size );

    return blob;
}

Parameter::Parameter()
    : m_name( "" )
    , m_type( PT_None )
    , m_numItems( 1 )
    , m_next( nullptr ) {
    // empty
}

Parameter::~Parameter() {
    // empty
}

ui32 Parameter::getParamDataSize( ParameterType type, ui32 arraySize ) {
    ui32 size( 0 );
    switch( type ) {
    case PT_Int:
        size = sizeof( i32 );
        break;
    case PT_Float:
        size = sizeof( f32 );
        break;
    case PT_Float2:
        size = sizeof( f32 ) * 2;
        break;
    case PT_Float3:
        size = sizeof( f32 ) * 3;
        break;
    case PT_Mat4:
        size = sizeof( f32 ) * 16;
        break;
    case PT_Mat4Array:
        size = sizeof( f32 ) * 16 * arraySize;
        break;

    default:
        break;
    }

    return size;
}

Parameter *Parameter::create( const String &name, ParameterType type, ui32 arraySize ) {
    Parameter *param = new Parameter;
    param->m_name = name;
    param->m_type = type;
    param->m_numItems = arraySize;
    param->m_data.m_size = Parameter::getParamDataSize( type, arraySize );
    param->m_data.m_data = new uc8[ param->m_data.m_size ];
    ::memset( param->m_data.m_data, 0, param->m_data.m_size );
    param->m_next = nullptr;

    return param;
}

} // Namespace RenderBackend
} // Namespace OSRE
