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

static const String Tag = "Parameter";

ParamDataBlob::ParamDataBlob()
: m_data( nullptr )
, m_size( 0 ) {
    // empty
}

ParamDataBlob::~ParamDataBlob() {
    clear();
}

void *ParamDataBlob::getData() const {
    return m_data;
}

void ParamDataBlob::clear() {
    delete[] m_data;
    m_data = nullptr;
    m_size = 0;
}

ParamDataBlob *ParamDataBlob::create( ParameterType type, ui32 arraySize ) {
    ParamDataBlob *blob = new ParamDataBlob;
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

Parameter::Parameter()
: m_name( "" )
, m_type( ParameterType::PT_None )
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

Parameter *Parameter::create( const String &name, ParameterType type, ui32 arraySize ) {
    if( name.empty() ) {
        osre_debug( Tag, "Empty name for parameter." );
        return nullptr;
    }

    Parameter *param     = new Parameter;
    param->m_name        = name;
    param->m_type        = type;
    param->m_numItems    = arraySize;
    param->m_data.m_size = Parameter::getParamDataSize( type, arraySize );
    param->m_data.m_data = new uc8[ param->m_data.m_size ];
    param->m_next        = nullptr;
    ::memset( param->m_data.m_data, 0, param->m_data.m_size );

    return param;
}

void Parameter::destroy( Parameter *param ) {
    if ( nullptr != param ) {
        delete param;
    }
}


ParameterRegistry *ParameterRegistry::s_instance = nullptr;

ParameterRegistry *ParameterRegistry::create( RenderBackendService *rbSrv ) {
    if ( nullptr == s_instance ) {
        s_instance = new ParameterRegistry( rbSrv );
    }
    return s_instance;
}

void ParameterRegistry::destroy() {
    if ( nullptr != s_instance ) {
        delete s_instance;
        s_instance = nullptr;
    }
}

bool ParameterRegistry::registerParameter( Parameter *param ) {
    if ( nullptr == s_instance ) {
        return false;
    }

    if ( nullptr == param ) {
        return false;
    }

    bool res( false );
    const ui32 hash( calcHash( param->m_name ) );
    if ( !s_instance->m_parameterMap.hasKey( hash ) ) {
        s_instance->m_parameterMap.insert( hash, param );
        res = true;
    }

    return res;
}

Parameter *ParameterRegistry::getParameterByName( const String & name ) {
    if ( nullptr == s_instance ) {
        return nullptr;
    }

    Parameter *param( nullptr );
    const ui32 hash( calcHash( name ) );
    if ( s_instance->m_parameterMap.hasKey( hash ) ) {
        s_instance->m_parameterMap.getValue( hash, param );
    }
    
    return param;
}

bool ParameterRegistry::updateParameter( Parameter *param ) {
    if ( nullptr == s_instance ) {
        return false;
    }

    if ( nullptr == param ) {
        return false;
    }
    
    const ui32 hash( calcHash( param->m_name ) );
    if ( !s_instance->m_parameterMap.hasKey( hash ) ) {
        return false;
    }

    s_instance->m_updates.add( param );
    
    return true;
}

bool ParameterRegistry::commitChanges() {
    if ( nullptr == s_instance ) {
        return false;
    }

    if ( s_instance->m_updates.isEmpty() ) {
        return true;
    }

    UpdateParameterEventData *data( new UpdateParameterEventData );
    data->m_numParam = s_instance->m_updates.size();
    data->m_param = new Parameter*[ data->m_numParam ];
    for ( ui32 i = 0; i < data->m_numParam; i++ ) {
        data->m_param[ i ] = s_instance->m_updates[ i ];
    }
    s_instance->m_updates.resize( 0 );

    s_instance->m_rbService->sendEvent( &OnUpdateParameterEvent, data );

    return true;
}

ParameterRegistry::ParameterRegistry( RenderBackendService *rbSrv )
: m_parameterMap()
, m_updates()
, m_rbService( rbSrv ) {
    OSRE_ASSERT( nullptr != rbSrv );
}

ParameterRegistry::~ParameterRegistry() {
    // empty
}

} // Namespace RenderBackend
} // Namespace OSRE
