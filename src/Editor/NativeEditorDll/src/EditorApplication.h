#pragma once

#include "EditorMain.h"
#include <osre/Common/Event.h>
#include <osre/Scene/Node.h>
#include <osre/App/AppBase.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/RenderBackend/RenderCommon.h>

namespace OSRE {

namespace Scene {
    class World;
    class Stage;
}

namespace Platform {
    class PlatformInterface;
}

namespace NativeEditor {

class EditorApplication;

class MouseEventListener : public Platform::OSEventListener {
    EditorApplication *m_app;
public:
    MouseEventListener( EditorApplication *app )
        : OSEventListener( "Editor/MouseEventListener" )
        , m_app( app ) {
        // app
    }

    ~MouseEventListener() {
        // empty
    }

    void onOSEvent( const Common::Event &osEvent, const Common::EventData *data ) override {
        if (osEvent == Platform::MouseButtonDownEvent) {
            if (nullptr != data) {
                osre_info( "MouseEventListener", "mouse click" );
            }
        }
    }
};

class EditorApplication : public App::AppBase {
    Scene::World *m_world;
    Scene::Stage *m_stage;
    Scene::Node::NodePtr m_modelNode;
    RenderBackend::TransformMatrixBlock m_transformMatrix;
    Platform::PlatformInterface *m_platformInterface;
    String m_projectName;

public:
    EditorApplication( int argc, char *argv[] );

    virtual ~EditorApplication();

    int enqueueEvent( const Common::Event *ev, Common::EventData *evData );

    void newProject( const String &name );

    Scene::World *getWorld() const;
    int importAsset( const String &filename, int flags );
    Platform::PlatformInterface *getPlatformInterface() const;
    bool loadWorld( const char *filelocation, int flags );
    bool saveWorld( const char *filelocation, int flags );

protected:
    bool onCreate( Properties::Settings *settings = nullptr ) override;
};

}
}
