#include "Grid.h"

namespace OSRE {
namespace NativeEditor {

using namespace ::OSRE::Scene;

Grid::Grid( Common::Ids &ids )
: Node( "grid", ids, Node::RenderCompRequest::RenderCompRequested, Node::TransformCompRequest::TransformCompRequested ) {
    // empty
}

Grid::~Grid() {
    // empty
}

}
}
