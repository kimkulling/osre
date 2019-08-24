#include <osre/Scene/Node2D.h>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;

Node2D::Node2D(const String& name, Common::Ids& ids, Node* parent)
: Node(name, ids, parent)
, m_canvas(nullptr ) {
    
}

Node2D::~Node2D() {

}

void Node2D::setCanvas(UI::Canvas* canvas) {
    m_canvas = canvas;
}

UI::Canvas *Node2D::getCanvas() const {
    return m_canvas;
}

void Node2D::onUpdate(Time dt) {

}

void Node2D::onRender(RenderBackend::RenderBackendService* rbSrv) {

}

}
}
