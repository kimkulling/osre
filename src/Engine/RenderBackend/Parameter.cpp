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
#include <osre/RenderBackend/Parameter.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Common/StringUtils.h>
#include <osre/Common/Logger.h>

namespace OSRE {
namespace RenderBackend {

using namespace ::OSRE::Common;

static const String Tag = "UniformVar";

UniformDataBlob::UniformDataBlob()
: m_data( nullptr )
, m_size( 0 ) {
    // empty
}

UniformDataBlob::~UniformDataBlob() {
    clear();
}

void *UniformDataBlob::getData() const {
    return m_data;
}

void UniformDataBlob::clear() {
    delete[] m_data;
    m_data = nullptr;
    m_size = 0;
}

UniformDataBlob *UniformDataBlob::create( ParameterType type, ui32 arraySize ) {
    UniformDataBlob *blob = new UniformDataBlob;
    switch( type ) {
        case ParameterType::PT_Int:
            blob->m_size = sizeof( i32 );
            break;
        case ParameterType::PT_Float:
            blob->m_size = sizeof( f32 );
            break;
        case ParameterType::PT_Float2:
            blob->m_size = sizeof( f32 ) * 2;
            break;
        case ParameterType::PT_Float3:
            blob->m_size = sizeof( f32 ) * 3;
            break;
        case ParameterType::PT_Mat4:
            blob->m_size = sizeof( f32 ) * 16;
            break;
        case ParameterType::PT_Mat4Array:
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

UniformVar::UniformVar()
: m_name( "" )
, m_type( ParameterType::PT_None )
, m_numItems( 1 )
, m_next( nullptr ) {
    // empty
}

UniformVar::~UniformVar() {
    // empty
}

ui32 UniformVar::getParamDataSize( ParameterType type, ui32 arraySize ) {
    ui32 size( 0 );
    switch( type ) {
        case ParameterType::PT_Int:
            size = sizeof( i32 );
            break;
        case ParameterType::PT_Float:
            size = sizeof( f32 );
            break;
        case ParameterType::PT_Float2:
            size = sizeof( f32 ) * 2;
            break;
        case ParameterType::PT_Float3:
            size = sizeof( f32 ) * 3;
            break;
        case ParameterType::PT_Mat4:
            size = sizeof( f32 ) * 16;
            break;
        case ParameterType::PT_Mat4Array:
            size = sizeof( f32 ) * 16 * arraySize;
            break;

        default:
            break;
    }

    return size;
}

static ui32 calcHash( const String &name ) {
    const ui32 hash( StringUtils::hashName( name.c_str() ) );
    return hash;
}

UniformVar *UniformVar::create( const String &name, ParameterType type, ui32 arraySize ) {
    if( name.empty() ) {
        osre_debug( Tag, "Empty name for parameter." );
        return nullptr;
    }

    UniformVar *param     = new UniformVar;
    param->m_name        = name;
    param->m_type        = type;
    param->m_numItems    = arraySize;
    param->m_data.m_size = UniformVar::getParamDataSize( type, arraySize );
    param->m_data.m_data = new uc8[ param->m_data.m_size ];
    param->m_next        = nullptr;
    ::memset( param->m_data.m_data, 0, param->m_data.m_size );

    return param;
}

void UniformVar::destroy( UniformVar *param ) {
    if ( nullptr != param ) {
        delete param;
    }
}

} // Namespace RenderBackend
} // Namespace OSRE
