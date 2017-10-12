/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2017 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Scene/DbgRenderer.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/Scene/GeometryBuilder.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;

DbgRenderer *DbgRenderer::s_instance = nullptr;

DbgRenderer::DbgRenderer( RenderBackend::RenderBackendService *rbSrv )
: m_rbSrv( rbSrv )
, m_textBoxes()
, m_tbArray() {
    OSRE_ASSERT( nullptr != m_rbSrv );
}

DbgRenderer::~DbgRenderer() {
    clearDbgTextCache();
}

bool DbgRenderer::create( RenderBackend::RenderBackendService *rbSrv ) {
    if ( nullptr != s_instance ) {
        return false;
    }
    s_instance = new DbgRenderer( rbSrv );
    return true;
}

bool DbgRenderer::destroy() {
    if ( nullptr == s_instance ) {
        return false;
    }
    delete s_instance;
    s_instance = nullptr;
    return true;
}

DbgRenderer *DbgRenderer::getInstance() {
    return s_instance;
}

static void insertTextEntry( ui32 id, Geometry *geo, const String &text, DbgRenderer::TextBoxHashMap &textBoxes ) {
    DbgRenderer::DbgTextEntry *entry( new DbgRenderer::DbgTextEntry );
    entry->m_geo = geo;
    entry->m_text = text;
    textBoxes.insert( id, entry );    
}

void DbgRenderer::renderDbgText( ui32 x, ui32 y, ui32 id, const String &text ) {
    if ( text.empty() ) {
        return;
    }

    if ( !m_textBoxes.hasKey( id ) ) {
        Geometry *geo = GeometryBuilder::allocTextBox( 0, 0, 0.1f, text, BufferAccessType::ReadWrite );
        m_rbSrv->attachGeo( geo, 0 );
        insertTextEntry( id, geo, text, m_textBoxes );

        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.m_model = glm::scale( m_transformMatrix.m_model, glm::vec3( .5, .5, .5 ) );
        m_transformMatrix.update();
        geo->m_model = m_transformMatrix.m_model;
    } else {
        DbgTextEntry *entry( nullptr );
        if ( m_textBoxes.getValue( id, entry ) ) {
            OSRE_ASSERT( nullptr != entry );
            if ( entry->m_text == text ) {
                return;
            }
            Geometry *geo( nullptr );
            if ( text.size() > entry->m_text.size() ) {
                geo = GeometryBuilder::allocTextBox( 0, 0, 0.1f, text, BufferAccessType::ReadWrite );
                entry->m_geo = geo;
            } else {
                GeometryBuilder::updateTextBox( entry->m_geo, 0.1f, text, false );
                geo = entry->m_geo;
            }
            m_rbSrv->attachGeoUpdate( geo );
            m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
            m_transformMatrix.m_model = glm::scale( m_transformMatrix.m_model, glm::vec3( .5, .5, .5 ) );
            m_transformMatrix.update();
            geo->m_model = m_transformMatrix.m_model;
        }
    }
}

void DbgRenderer::clearDbgTextCache() {
    if ( m_textBoxes.isEmpty() ) {
        return;
    }

    for ( ui32 i = 0; i < m_tbArray.size(); i++ ) {
        if ( nullptr != m_tbArray[ i ] ) {
            delete m_tbArray[ i ];
        }
    }
    m_textBoxes.clear();
    m_textBoxes.init(100);
    m_tbArray.resize( 0 );
    m_textBoxes.init( 100 );
}

ui32 DbgRenderer::numDbgTexts() const {
    return m_tbArray.size();
}

} // Namespace Scene
} // namespace OSRE
