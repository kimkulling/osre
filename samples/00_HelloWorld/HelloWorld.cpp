#include <osre/App/AppBase.h>
#include <osre/Properties/ConfigurationMap.h>
#include <osre/Common/Logger.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/RenderBackend/RenderCommon.h>

using namespace OSRE;
using namespace OSRE::RenderBackend;

// to identify local log entries 
static const String Tag    = "HelloWorld"; 

class HelloWorldApp : public App::AppBase {
    Scene::Stage *m_stage;
public:
    HelloWorldApp( int argc, char *argv[] )
    : AppBase( argc, argv )
    , m_stage( nullptr ) {

    }

    virtual ~HelloWorldApp() {

    }

    virtual bool create( Properties::ConfigurationMap *config = nullptr ) {
        AppBase::getConfig()->setString( Properties::ConfigurationMap::WindowsTitle, "HelloWorld!" );

        m_stage = new Scene::Stage( "HelloWorld" );
        Scene::Node *geoNode = m_stage->addNode( "geo", nullptr );
        Scene::GeometryBuilder myBuilder;
        RenderBackend::Geometry *geo = myBuilder.createTriangle();
        geoNode->addGeometry( geo );

        return AppBase::create( config );
    }
};

int main( int argc, char *argv[] )  {
    HelloWorldApp myApp( argc, argv );
    if( !myApp.create() ) {
        osre_error(Tag, "Cannot create application instance." );
        return 1;
    }
    
    //attachGeoEvData->m_numGeo = 1;
    //attachGeoEvData->m_pGeometry = pGeometry;

    // handle events until application will be terminated
    while( myApp.handleEvents() ) {
        
        // update application data
        myApp.update();

        // request next frame rendering
        myApp.requestNextFrame();
    }
    myApp.destroy();

    return 0;
}
