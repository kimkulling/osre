#include <osre/Scene/DbgRenderer.h>
#include <osre/RenderBackend/RenderBackendService.h>
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
, m_textBoxes() {
    OSRE_ASSERT( nullptr != m_rbSrv );
}

DbgRenderer::~DbgRenderer() {

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

void DbgRenderer::renderDbgText( ui32 x, ui32 y, ui32 id, const String &text ) {
    if ( text.empty() ) {
        return;
    }

    if ( !m_textBoxes.hasKey( id ) ) {
        AttachGeoEventData *attachGeoEvData = new AttachGeoEventData;
        Geometry *geo = GeometryBuilder::allocTextBox( 0, 0, 0.1f, text );
        
        DbgTextEntry *entry( new DbgTextEntry );
        entry->m_geo = geo;
        entry->m_text = text;
        m_textBoxes.insert( id, entry );

        attachGeoEvData->m_numGeo = 1;
        attachGeoEvData->m_geo = geo;

        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.m_model = glm::scale( m_transformMatrix.m_model, glm::vec3( .5, .5, .5 ) );
        Parameter *parameter = Parameter::create( "MVP", PT_Mat4 );
        const float *mvpData( glm::value_ptr( m_transformMatrix.m_projection*m_transformMatrix.m_view*m_transformMatrix.m_model ) );
        ::memcpy( parameter->m_data.m_data, mvpData, sizeof( glm::mat4 ) );

        geo->m_material->m_parameters = parameter;
        geo->m_material->m_numParameters++;

        m_rbSrv->sendEvent( &OnAttachSceneEvent, attachGeoEvData );

    } else {
        DbgTextEntry *entry( nullptr );
        if ( m_textBoxes.getValue( id, entry ) ) {
            if ( entry->m_text == text ) {
                return;
            }
            
            UpdateGeoEventData *updateGeoEvData( new UpdateGeoEventData );
            updateGeoEvData->m_numGeo = 1;
            if ( text.size() > entry->m_text.size() ) {
                Geometry *geo = GeometryBuilder::allocTextBox( -1, -1, 0.1f, text );
                updateGeoEvData->m_geo = geo;
            } else {
                GeometryBuilder::updateTextBox( entry->m_geo, 0.1f, text, false );
                updateGeoEvData->m_geo = entry->m_geo;
            }
            m_rbSrv->sendEvent( &OnUpdateGeoEvent, updateGeoEvData );
        }
    }
}

} // Namespace Scene
} // namespace OSRE
