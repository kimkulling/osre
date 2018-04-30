#include "EditorMain.h"

#include <osre/Common/osre_common.h>
#include <osre/App/AppBase.h>
#include <osre/Properties/Settings.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/World.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>

#include <osre/Platform/Windows/MinWindows.h>

using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Properties;
using namespace ::OSRE::Scene;

class EditorApplication : public App::AppBase {
    World *m_world;
    Stage *m_stage;
    TransformMatrixBlock m_transformMatrix;

public:
    EditorApplication(int argc, char *argv[])
    : AppBase(argc, argv)
    , m_world( nullptr )
    , m_stage( nullptr ) {
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

        m_world = new World("HelloWorld", RenderMode::Render3D);
        m_stage = AppBase::createStage( "HelloWorld" );
        m_world->addStage(m_stage);
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

extern "C" OSRE_EDITOR_EXPORT int __stdcall CreateEditorApp( int *mainWindowHandle ) {
    if (nullptr == s_EditorApplication) {
        HWND mainWH( nullptr );
        if ( nullptr != mainWindowHandle ) {
            mainWH = (HWND) mainWindowHandle;
        }
        char *argc[] = { "CreateEditorApp" };
        s_EditorApplication = new EditorApplication(1, argc );
        s_EditorApplication->create();
        s_EditorApplication->update();
        s_EditorApplication->requestNextFrame();

    }

    return 0;
}

int __stdcall EditorUpdate() {
    if ( nullptr == s_EditorApplication ) {
        return 1;
    }

    s_EditorApplication->update();
    
    return 0;
}

int __stdcall EditorRequestNextFrame() {
    if ( nullptr == s_EditorApplication ) {
        return 1;
    }
    s_EditorApplication->requestNextFrame();

    return 0;
}

int __stdcall DestroyEditorApp() {
    if ( nullptr == s_EditorApplication ) {
        return 1;
    }
    s_EditorApplication->destroy();
    delete s_EditorApplication;
    s_EditorApplication = nullptr;

    return 0;
}

int __stdcall LoadWorld(const char *filelocation, int flags) {
    if (nullptr == filelocation) {
        return 1;
    }

    return 0;
}

int __stdcall SaveWorld(const char *filelocation, int flags) {
    if (nullptr == filelocation) {
        return 1;
    }

    return 0;
}
