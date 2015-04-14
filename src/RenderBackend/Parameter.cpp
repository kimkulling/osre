/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms,
with or without modification, are permitted provided that the
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions
and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
and the following disclaimer in the documentation and/or other materials provided with the
distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or
promote products derived from this software without specific prior written permission of the
ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
-------------------------------------------------------------------------------------------------*/
#include <zfxce2/RenderSystem/RenderBackend/Parameter.h>


namespace ZFXCE2 {
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

Parameter *Parameter::create( const ce_string &name, ParameterType type, ui32 arraySize ) {
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
} // Namespace ZFXCE2
