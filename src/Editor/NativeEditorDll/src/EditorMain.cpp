#include "EditorMain.h"

#include <osre/Common/osre_common.h>
#include <osre/App/AppBase.h>
#include <osre/Properties/Settings.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>

using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Properties;

class EditorApplication : public App::AppBase {
    Scene::Stage *m_stage;
    TransformMatrixBlock m_transformMatrix;

public:
    EditorApplication(int argc, char *argv[])
    : AppBase(argc, argv) {
        // empty
    }

    virtual ~EditorApplication() {
        // empty
    }

protected:
    bool onCreate(Properties::Settings *settings = nullptr) override {
        Properties::Settings *baseSettings(AppBase::getSettings());
        if (nullptr == baseSettings) {
            return false;
        }

        baseSettings->setString(Properties::Settings::WindowsTitle, "Demo UI!");
        if (!AppBase::onCreate(baseSettings)) {
            return false;
        }

        m_stage = AppBase::createStage( "HelloWorld" );
        AppBase::activateStage( m_stage->getName() );

        Scene::Node *geoNode = m_stage->addNode( "geo", nullptr );
        const Scene::Node::AABB &aabb = geoNode->getAABB();

        Scene::GeometryBuilder myBuilder;
        RenderBackend::Geometry *geo = myBuilder.allocTriangles( VertexType::ColorVertex, BufferAccessType::ReadOnly );
        if ( nullptr != geo ) {
            m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
            m_transformMatrix.update();
            getRenderBackendService()->setMatrix( "MVP", m_transformMatrix.m_mvp );
            geoNode->addGeometry( geo );
        }


        return true;
    }
};

static EditorApplication *s_EditorApplication = nullptr;

extern "C" OSRE_EDITOR_EXPORT int CreateEditorApp() {
    if (nullptr == s_EditorApplication) {
        char *argc[] = { "CreateEditorApp" };
        s_EditorApplication = new EditorApplication(1, argc );
        s_EditorApplication->create();
        s_EditorApplication->update();
        s_EditorApplication->requestNextFrame();

    }

    return 0;
}

int EditorUpdate() {
    s_EditorApplication->update();
    
    return 0;
}

int EditorRequestNextFrame() {
    s_EditorApplication->requestNextFrame();

    return 0;
}

int DestroyEditorApp() {
    s_EditorApplication->destroy();
    delete s_EditorApplication;
    s_EditorApplication = nullptr;

    return 0;
}
