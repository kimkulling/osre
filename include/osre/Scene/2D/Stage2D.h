#pragma once

#include <osre/Scene/Stage.h>

namespace OSRE {
namespace Scene {

class Stage2D : public Stage {
public:
    Stage2D(  const String &name, RenderBackend::RenderBackendService *rbService );
    virtual ~Stage2D();
    virtual Node *addNode( const String &name, Node *parent, const String &type="default" );
};

} // Namespace Scene
} // Namespace OSRE
