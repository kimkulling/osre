#include "EditorApp.h"

using namespace ::OSRE;

int main( int argc, char *argv[] ) {
    EditorApp myApp( argc, argv );
    if ( !myApp.create() ) {
        return 1;
    }

    while ( myApp.handleEvents() && !myApp.shutdownRequested() ) {
        myApp.update();
        myApp.requestNextFrame();
    }
    myApp.destroy();
    
    return 0;
}
