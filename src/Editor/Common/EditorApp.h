#pragma once

#include <osre/App/AppBase.h>
#include <osre/IO/Uri.h>
#include <osre/RenderBackend/RenderBackendService.h>

namespace OSRE {

    namespace Assets {
        class AssetDataArchive;
    }

    namespace Scene {
        class World;
    }

    namespace UI {
        class Screen;
        class Panel;
    }

    class EditorApp : public App::AppBase {
    public:
        EditorApp( int argc, char *argv[] );
        virtual ~EditorApp();
        bool openWorld( const String &projectName );
        bool closeWorld();
        bool loadWorld( const IO::Uri &loc );
        bool saveWorld( const IO::Uri &loc );

    protected:
        void newProjectCallback( ui32 id, void *data );
        void openProjectCallback( ui32 id, void *data );
        void closeProjectCallback( ui32 id, void *data );
        void quitEd( ui32 id, void *data );
        void setupProjectPanel();
        void setupUI();
        void setupRenderArea();
        virtual bool onCreate( Properties::Settings *settings = nullptr );
        bool onLoadWorld();
        bool onSaveWorld();
        bool onImportStage();
        void setProjectTitle( const String &project );

    private:
        Scene::World * m_world;
        Assets::AssetDataArchive  *m_project;
        IO::Uri            m_projecUri;
        UI::Screen        *m_screen;
        UI::Panel         *m_panel;
        RenderBackend::TransformMatrixBlock m_transformMatrix;
    };
}
