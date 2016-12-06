#include <osre/Scene/2D/Stage2D.h>
#include <osre/Scene/2D/Node2D.h>

namespace OSRE {
namespace Scene {

Stage2D::Stage2D(  const String &name, RenderBackend::RenderBackendService *rbService  )
: Stage( name, rbService ) {
    // empty
}

Stage2D::~Stage2D() {
    // empty
}

Node *Stage2D::addNode( const String &name, Node *parent, const String &type ) {
    if ( "default" == type ) {
        Common::Ids *ids = Stage::getIdContainer();
        if ( nullptr== ids ) {
            return nullptr;
        }
        return new Node2D( name, *ids, true, true, parent );
    } else {
        return Stage::addNode( name, parent, type );
    }
}


} // Namespace Scene
} // Namespace OSRE
