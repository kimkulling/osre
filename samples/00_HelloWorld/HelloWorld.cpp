#include <osre/App/AppBase.h>
#include <osre/Common/Logger.h>

using namespace OSRE;

// to identify local log entries 
static const String Tag    = "HelloWorld"; 

int main( int argc, char *argv[] )  {
    App::AppBase myApp( argc, argv );
    String renderAPI( "opengl" );

    if( !myApp.create() ) {
        osre_error(Tag, "Cannot create application instance." );
        return 1;
    }
        
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