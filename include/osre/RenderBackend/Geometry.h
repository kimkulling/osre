#pragma once

#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {
namespace RenderBackend {

struct OSRE_EXPORT Geometry {
    bool            m_localMatrix;
    glm::mat4       m_model;
    Material       *m_material;
    VertexType      m_vertextype;
    BufferData     *m_vb;
    IndexType       m_indextype;
    BufferData     *m_ib;
    ui32            m_numPrimGroups;
    PrimitiveGroup *m_pPrimGroups;
    ui32            m_id;

    static Geometry *create( ui32 numGeo );
    static void destroy( Geometry **geo );
    static ui32 getVertexSize( VertexType vertextype );

    OSRE_NON_COPYABLE( Geometry )

private:
    Geometry();
    ~Geometry();
};

} // Namespace RenderBackend
} // Namespace OSRE

