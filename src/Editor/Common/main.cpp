#include <osre/App/AppBase.h>
#include <osre/Common/Logger.h>
#include <osre/Common/ArgumentParser.h>
#include <osre/Properties/Settings.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/View.h>
#include <osre/Common/ArgumentParser.h>
#include <osre/Scene/World.h>

using namespace ::OSRE;
using namespace ::OSRE::Common;
using namespace ::OSRE::Scene;

static const String SupportedArgs = "help:api:gen_project:asset_path";
static const String Descs         = "Shows the help:The render API:Generates a template project:Path to media";
static const String Tag = "osre_ed";

class osre_ed : public App::AppBase {
    World *m_world;

public:
    osre_ed( int argc, char *argv[] )
    : AppBase( argc, argv, SupportedArgs, Descs )
    , m_world( nullptr ) {
        // empty
    }

    virtual ~osre_ed() {
        // empty
    }

    bool loadWorld() {
        return false;
    }

    bool saveWorld() {
        return false;
    }

protected:
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
