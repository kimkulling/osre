#pragma once

#include <osre/Common/osre_common.h>
#include <osre/Scene/Node.h>

namespace OSRE {
    
    namespace Common {
        class Ids;
    }

    namespace UI {
        class Canvas;
    }

    namespace Scene {

class Node2D : public Node {
public:
    Node2D(const String& name, Common::Ids& ids, Node* parent = nullptr);
    ~Node2D();
    void setCanvas(UI::Canvas* canvas);
    UI::Canvas* getCanvas() const;

protected:
    void onUpdate(Time dt) override;
    void onRender(RenderBackend::RenderBackendService *rbSrv) override;

private:
    UI::Canvas *m_canvas;
};

}
}
