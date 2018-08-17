#include "EditorMain.h"

#include <osre/Common/osre_common.h>
#include <osre/Common/Ids.h>
#include <osre/Common/Event.h>
#include <osre/App/AppBase.h>
#include <osre/Properties/Settings.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Platform/AbstractPlatformEventQueue.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/World.h>
#include <osre/IO/IOService.h>
#include <osre/Assets/AssimpWrapper.h>
#include <osre/Assets/Model.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>

#include <src/Engine/Platform/win32/Win32Window.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iomanip>

#ifdef OSRE_WINDOWS
#   include <osre/Platform/Windows/MinWindows.h>
#endif // OSRE_WINDOWS

#include "SceneNodeIterator.h"

using namespace ::OSRE;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Properties;
using namespace ::OSRE::Platform;
using namespace ::OSRE::Scene;

class EditorApplication;

static const String Tag = "EditorMain";

class MouseEventListener : public Platform::OSEventListener {
    EditorApplication *m_app;
public:
    MouseEventListener(EditorApplication *app)
    : OSEventListener("Editor/MouseEventListener")
    , m_app( app ) {
        // app
    }

    ~MouseEventListener() {
        // empty
    }

    void onOSEvent(const Event &osEvent, const EventData *data) override {
        if (osEvent == Platform::MouseButtonDownEvent ) {
            if (nullptr != data) {
                osre_info(Tag, "mouse click");
            }
        }
    }
};

class EditorApplication : public App::AppBase {
    World *m_world;
    Stage *m_stage;
    Node::NodePtr m_modelNode;
    TransformMatrixBlock m_transformMatrix;
    Platform::PlatformInterface *m_platformInterface;
    String m_projectName;

public:
    EditorApplication(int argc, char *argv[])
    : AppBase(argc, argv)
    , m_world( nullptr )
    , m_stage( nullptr )
    , m_modelNode()
    , m_transformMatrix()
    , m_platformInterface( nullptr )
    , m_projectName( "untitled" ) {
        // empty
    }

    virtual ~EditorApplication() {
        // empty
    }

    int enqueueEvent(const Common::Event *ev, const Common::EventData *evData) {
        return 0;
    }

    void newProject( const String &name) {
        if (name != m_projectName) {
            m_projectName = name;
        }
        Platform::AbstractWindow *rootWindow(getRootWindow());
        if (nullptr != rootWindow) {
            rootWindow->setWindowsTitle(m_projectName);
        }
    }

    World *getWorld() const {
        return m_world;
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
            Stage *stage = AppBase::createStage(model->getRootNode()->getName());
            stage->setRoot( model->getRootNode() );
            m_world->addStage(stage);
            m_world->setActiveStage(stage);
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

            }
            m_stage->setRoot( model->getRootNode() );

            update();
            requestNextFrame();
        }

        return 0;
    }

    PlatformInterface *getPlatformInterface() const {
        return m_platformInterface;
    }

protected:
    bool onCreate( Properties::Settings *settings = nullptr ) override {
        Properties::Settings *baseSettings(settings);
        if (baseSettings == nullptr) {
            baseSettings = AppBase::getSettings();
            if (nullptr == baseSettings) {
                return false;
            }
        }
        baseSettings->setBool( Settings::ChildWindow, true );
        baseSettings->setString(Properties::Settings::WindowsTitle, m_projectName );
        if (!AppBase::onCreate(baseSettings)) {
            return false;
        }

        m_world = new World("HelloWorld", RenderMode::Render3D);
        m_stage = AppBase::createStage( "HelloWorld" );
        m_world->addStage(m_stage);

        m_platformInterface = Platform::PlatformInterface::getInstance();
        AppBase::activateStage( m_stage->getName() );

        return true;
    }
};

static EditorApplication *s_EditorApplication = nullptr;

extern "C" OSRE_EDITOR_EXPORT int STDCALL CreateEditorApp( int *mainWindowHandle ) {
#ifdef OSRE_WINDOWS
    if (nullptr == s_EditorApplication) {
        HWND mainWH( nullptr );
        if ( nullptr != mainWindowHandle ) {
            mainWH = (HWND) mainWindowHandle;
        }
        char *argc[] = { "CreateEditorApp" };
        s_EditorApplication = new EditorApplication(1, argc );
        s_EditorApplication->create();
        s_EditorApplication->update();

        Win32Window *window = (Win32Window*) s_EditorApplication->getRootWindow();
        if (nullptr != window) {
            HWND childHandle = window->getHWnd();
            ::SetParent( childHandle, mainWH );
            RECT rect;
            ::GetClientRect( mainWH, &rect );
            const ui32 w = rect.right - rect.left;

            ::MoveWindow( childHandle, 25, 45, w - 240, rect.bottom - 45, TRUE );
        }

        ::CPPCore::TArray<const Common::Event*> eventArray;
        eventArray.add( &KeyboardButtonDownEvent );
        OSEventListener *listener = new MouseEventListener(s_EditorApplication);
        AbstractPlatformEventQueue *evQueue = s_EditorApplication->getPlatformInterface()->getPlatformEventHandler();
        evQueue->registerEventListener(eventArray, listener);

        s_EditorApplication->requestNextFrame();

    }
#endif // OSRE_WINDOWS
    return 0;
}

int STDCALL EditorUpdate() {
    if ( nullptr == s_EditorApplication ) {
        return 1;
    }

    s_EditorApplication->update();
    
    return 0;
}

int STDCALL EditorRequestNextFrame() {
    if ( nullptr == s_EditorApplication ) {
        return 1;
    }

    s_EditorApplication->requestNextFrame();

    return 0;
}

int STDCALL DestroyEditorApp() {
    if ( nullptr == s_EditorApplication ) {
        return 1;
    }
    s_EditorApplication->destroy();
    delete s_EditorApplication;
    s_EditorApplication = nullptr;

    return 0;
}

int STDCALL NewProject(const char *name) {
    if (nullptr == name) {
        return 1;
    }
    s_EditorApplication->newProject(name);

    return 0;
}

int STDCALL LoadWorld(const char *filelocation, int flags) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filelocation) {
        return 1;
    }

    return 0;
}

int STDCALL SaveWorld(const char *filelocation, int flags) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filelocation) {
        return 1;
    }

    return 0;
}

int STDCALL ImportAsset(const char *filename, int flags) {
    if (nullptr == s_EditorApplication) {
        return 1;
    }

    if (nullptr == filename) {
        return 1;
    }

    return s_EditorApplication->importAsset(filename, flags);
}
