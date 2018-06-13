#include "EditorMain.h"

#include <osre/Common/osre_common.h>
#include <osre/Common/Ids.h>
#include <osre/App/AppBase.h>
#include <osre/Properties/Settings.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/World.h>
#include <osre/IO/IOService.h>
#include <osre/Assets/AssimpWrapper.h>
#include <osre/Assets/Model.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>

#ifdef OSRE_WINDOWS
#   include <osre/Platform/Windows/MinWindows.h>
#endif // OSRE_WINDOWS

using namespace ::OSRE;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Properties;
using namespace ::OSRE::Scene;

class EditorApplication : public App::AppBase {
    World *m_world;
    Stage *m_stage;
    Node::NodePtr m_modelNode;
    TransformMatrixBlock m_transformMatrix;

public:
    EditorApplication(int argc, char *argv[])
    : AppBase(argc, argv)
    , m_world( nullptr )
    , m_stage( nullptr )
    , m_modelNode()
    , m_transformMatrix() {
        // empty
    }

    virtual ~EditorApplication() {
        // empty
    }

    int importAsset( const String &filename, int flags ) {
        if (filename.empty() ) {
            return 1;
        }

        Ids ids;
        Assets::AssimpWrapper assimpWrapper(ids);
        String normalizedFilename;
#ifdef OSRE_WINDOWS
        IO::Uri::normalizePath(filename, '\\', normalizedFilename);
#else
        normalizedFilename = filename;
#endif
        IO::Uri modelLoc(normalizedFilename);
        if (assimpWrapper.importAsset(modelLoc, flags)) {
            Assets::Model *model = assimpWrapper.getModel();
            Collision::TAABB<f32> aabb = model->getAABB();
            const f32 diam = aabb.getDiameter();
            const Vec3f center = aabb.getCenter();

            CPPCore::TArray<Geometry*> geoArray = model->getGeoArray();
            m_transformMatrix.m_model = glm::rotate(m_transformMatrix.m_model, 0.0f, glm::vec3(1, 1, 0));
            m_transformMatrix.update();
            RenderBackendService *rbSrv(getRenderBackendService());
            if (nullptr != rbSrv) {
                Platform::AbstractWindow *rootWindow(getRootWindow());
                if (nullptr == rootWindow) {
                    return false;
                }
                const i32 w = rootWindow->getProperties()->m_width;
                const i32 h = rootWindow->getProperties()->m_height;
                const f32 aspect = static_cast<f32>(w) / static_cast<f32>(h);
                const f32 zNear = 0.0001f;
                const f32 zFar = 1000.f;

                glm::vec3 eye(diam, 0, 0), up(0, 0, 1);
                glm::vec3 c(center.getX(), center.getY(), center.getZ());

                m_transformMatrix.m_model = glm::mat4(1.0f);
                m_transformMatrix.m_projection = glm::perspective(glm::radians(60.0f), aspect, zNear, zFar);
                m_transformMatrix.m_view = glm::lookAt(eye, c, up);
                rbSrv->setMatrix(MatrixType::View, m_transformMatrix.m_view);
                rbSrv->setMatrix(MatrixType::Projection, m_transformMatrix.m_projection);

                renderNodes(model, rbSrv);
            }
            const String name(model->getRootNode()->getName());
            m_modelNode = m_stage->addNode(name, nullptr, "default");

            update();
            requestNextFrame();
        }
    }

    void renderNode(Node *currentNode, RenderBackendService *rbSrv) {
        const ui32 numGeo = currentNode->getNumGeometries();
        for (ui32 i = 0; i < numGeo; ++i) {
            rbSrv->attachGeo(currentNode->getGeometryAt(i), 0);
        }

        for (ui32 i = 0; i < currentNode->getNumChildren(); ++i) {
            Node *current = currentNode->getChildAt(i);
            renderNode(current, rbSrv);
        }
    }

    void renderNodes(Assets::Model *model, RenderBackendService *rbSrv) {
        if (nullptr == model) {
            return;
        }

        Node *root = model->getRootNode();
        if (nullptr == root) {
            return;
        }

        renderNode(root, rbSrv);
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
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filelocation) {
        return 1;
    }

    return 0;
}

int __stdcall SaveWorld(const char *filelocation, int flags) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filelocation) {
        return 1;
    }

    return 0;
}

int __stdcall ImportAsset(const char *filename, int flags) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filename) {
        return 1;
    }

    return s_EditorApplication->importAsset(filename, flags);
}

int __stdcall EnqueueEvent() {
    return 0;
}
