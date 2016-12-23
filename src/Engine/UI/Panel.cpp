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
#include <osre/UI/Panel.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace UI {

using namespace ::OSRE::RenderBackend;

Panel::Panel( const String &name, Widget *parent )
: Widget( name, parent ) {
}

Panel::~Panel() {

}

void Panel::onRender( TargetGeoArray &targetGeoArray, RenderBackend::RenderBackendService *rbSrv ) {
    Geometry *targetGeo = Geometry::create( 1 );
    const RectUI &rect( getRect() );
    static const ui32 NumVert = 4;
    RenderVert v[ NumVert ];
    v[ 0 ].position.x = static_cast<f32>( rect.m_x );
    v[ 0 ].position.y = static_cast<f32>( rect.m_y );

    v[ 1 ].position.x = static_cast<f32>( rect.m_x );
    v[ 1 ].position.y = static_cast<f32>( rect.m_y + rect.m_height );

    v[ 2 ].position.x = static_cast<f32>( rect.m_x + rect.m_width );
    v[ 2 ].position.y = static_cast<f32>( rect.m_y );

    v[ 3 ].position.x = static_cast<f32>( rect.m_x + rect.m_width );
    v[ 3 ].position.y = static_cast<f32>( rect.m_y + rect.m_height );
    const ui32 vbSize = sizeof( RenderVert ) * NumVert;
    targetGeo->m_vb = BufferData::alloc( BufferType::VertexBuffer, vbSize, BufferAccessType::ReadOnly );
    targetGeo->m_vb->copyFrom( &v[ 0 ], vbSize );
    
    static const ui32 NumIndices = 6;
    ui16 indices[ NumIndices ];

    indices[ 0 ] = 0;
    indices[ 1 ] = 1;
    indices[ 2 ] = 2;

    indices[ 3 ] = 1;
    indices[ 4 ] = 2;
    indices[ 5 ] = 3;

    ui32 ibSize = sizeof( ui16 ) * NumIndices;
    targetGeo->m_ib = BufferData::alloc( BufferType::IndexBuffer, ibSize, BufferAccessType::ReadOnly );
    targetGeo->m_vb->copyFrom( &indices[ 0 ], ibSize );

    targetGeo->m_numPrimGroups = 1;
    targetGeo->m_pPrimGroups = new PrimitiveGroup[ targetGeo->m_numPrimGroups ];
    targetGeo->m_pPrimGroups[ 0 ].m_indexType = IndexType::UnsignedShort;
    targetGeo->m_pPrimGroups[ 0 ].m_numPrimitives = 3 * targetGeo->m_numPrimGroups;
    targetGeo->m_pPrimGroups[ 0 ].m_primitive = PrimitiveType::TriangleList;
    targetGeo->m_pPrimGroups[ 0 ].m_startIndex = 0;

    targetGeoArray.add( targetGeo );
}

} // Namespace UI
} // Namespace OSRE
