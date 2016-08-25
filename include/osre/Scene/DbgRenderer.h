#pragma once

#include <osre/Common/osre_common.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <cppcore/Container/THashMap.h>

namespace OSRE {
    
// Forward declarations
namespace RenderBackend {
    class RenderBackendService;

    struct Geometry;
}

namespace Scene {

    
//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT DbgRenderer {
public:
    void renderDbgText( ui32 x, ui32 y, ui32 id, const String &text );

    static bool create( RenderBackend::RenderBackendService *rbSrv );
    static bool destroy();
    static DbgRenderer *getInstance();

private:
    DbgRenderer( RenderBackend::RenderBackendService *rbSrv );
    ~DbgRenderer();

private:
    static DbgRenderer *s_instance;
    RenderBackend::RenderBackendService *m_rbSrv;
    RenderBackend::TransformMatrixBlock m_transformMatrix;
    struct DbgTextEntry {
        String m_text;
        RenderBackend::Geometry *m_geo;
    };
    typedef CPPCore::THashMap<ui32, DbgTextEntry*> TextBoxHashMap;
    TextBoxHashMap m_textBoxes;
};

} // Namespace Scene
} // namespace OSRE
