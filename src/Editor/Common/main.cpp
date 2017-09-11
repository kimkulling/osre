#include <osre/App/AppBase.h>
#include <osre/Common/Logger.h>
#include <osre/Common/ArgumentParser.h>
#include <osre/Properties/Settings.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/Scene/World.h>
#include <osre/IO/Uri.h>
#include <osre/Assets/AssetDataArchive.h>
#include <osre/Platform/PlatformOperations.h>
#include <osre/UI/Panel.h>
#include <osre/UI/ButtonBase.h>
#include <osre/UI/Screen.h>
#include <osre/RenderBackend/RenderBackendService.h>

using namespace ::OSRE;
using namespace ::OSRE::Common;
using namespace ::OSRE::Scene;
using namespace ::OSRE::Platform;
using namespace ::OSRE::Assets;

static const String SupportedArgs = "help:api:gen_project:asset_path";
static const String Descs         = "Shows the help:The render API:Generates a template project:Path to media";
static const String Tag = "osre_ed";

class osre_ed : public App::AppBase {
    World             *m_world;
    AssetDataArchive  *m_project;
    IO::Uri            m_projecUri;
    UI::Screen        *m_screen;
    UI::Panel         *m_panel;
    RenderBackend::TransformMatrixBlock m_transformMatrix;

public:
    osre_ed( int argc, char *argv[] )
    : AppBase( argc, argv, SupportedArgs, Descs )
    , m_world( nullptr )
    , m_project( nullptr )
    , m_projecUri() {
        // empty
    }

    virtual ~osre_ed() {
        // empty
    }

    bool openWorld( const String &projectName ) {
        m_project = new AssetDataArchive( 1 );
        m_project->setName( projectName );

        return true;
    }

    bool closeWorld() {
        if ( nullptr == m_project ) {
            return true;
        }

        return true;
    }

    bool loadWorld( const IO::Uri &loc ) {
        if ( loc.isEmpty() ) {
            PlatformOperations::getFileOpenDialog( AssetDataArchive::getExtension().c_str(), m_projecUri );
        } else {
            m_projecUri = loc;
        }

        const String &name = m_projecUri.getResource();
        if ( name.empty() ) {
            return false;
        }

        m_project->setName( name );

        return true;
    }

    bool saveWorld( const IO::Uri &loc ) {
        if ( !loc.isValid() ) {
            PlatformOperations::getFileOpenDialog( AssetDataArchive::getExtension().c_str(), m_projecUri );
        } else {
            m_projecUri = loc;
        }

        return false;
    }

protected:
    void setupUI() {
        m_screen = AppBase::createScreen("OSRE-Editor");

        m_panel = new UI::Panel( "file_panel", m_screen );
        m_panel->setRect(10, 10, 500, 80);
        ;
        m_panel->addChildWidget( &UI::ButtonBase::createBaseButton("Open File", m_panel)->setRect(12, 12, 100, 78) );
        m_panel->addChildWidget(new UI::ButtonBase("Close", m_panel ) );
    }

    virtual bool onCreate( Properties::Settings *settings = nullptr ) {
        const ArgumentParser &argParser = getArgumentParser();
        if ( argParser.hasArgument( "help" ) ) {
            String help( argParser.showHelp() ); 
            osre_info( Tag, help );
            return false;
        }

        Properties::Settings *baseSettings( AppBase::getSettings() );
        if ( nullptr == baseSettings ) {
            return false;
        }

        baseSettings->setString( Properties::Settings::WindowsTitle, "osre_ed!" );
        if ( !AppBase::onCreate( baseSettings ) ) {
            return false;
        }

#ifdef OSRE_WINDOWS
        Assets::AssetRegistry::registerAssetPath( "assets", "../../media" );
#else
        Assets::AssetRegistry::registerAssetPath( "assets", "../media" );
#endif // OSRE_WINDOWS

        setupUI();

        m_transformMatrix.m_model = glm::rotate(m_transformMatrix.m_model, 0.0f, glm::vec3(1, 1, 0));
        m_transformMatrix.update();
        AppBase::getRenderBackendService()->setMatrix("MVP", m_transformMatrix.m_mvp);

        return true;
    }

    bool onLoadWorld() {
        return false;
    }

    bool onSaveWorld() {
        return false;
    }

    bool onImportStage() {
        return false;
    }
};

OSRE_MAIN( osre_ed )
