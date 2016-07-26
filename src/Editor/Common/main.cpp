#include <osre/App/AppBase.h>
#include <osre/Common/Logger.h>
#include <osre/Properties/Settings.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/View.h>
#include <osre/Common/ArgumentParser.h>
#include <osre/Scene/World.h>

using namespace ::OSRE;
using namespace ::OSRE::Common;
using namespace ::OSRE::Scene;

class osre_ed : public App::AppBase {
    World *m_world;

public:
    osre_ed( int argc, char *argv[] )
    : AppBase( argc, argv )
    , m_world( nullptr ) {
        // empty
    }

    virtual ~osre_ed() {
        // empty
    }

protected:
    virtual bool onCreate( Properties::Settings *settings = nullptr ) {
        Properties::Settings *baseSettings( AppBase::getSettings() );
        if ( nullptr == baseSettings ) {
            return false;
        }

        baseSettings->setString( Properties::Settings::WindowsTitle, "osre_ed!" );
        if ( !AppBase::onCreate( baseSettings ) ) {
            return false;
        }

#ifdef OSRE_WINDOWS
        Assets::AssetRegistry::getInstance()->registerAssetPath( "assets", "../../media" );
#else
        Assets::AssetRegistry::getInstance()->registerAssetPath( "assets", "../media" );
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
