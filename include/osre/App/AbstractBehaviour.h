#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace App {

class AbstractBehaviour {
public:
    virtual ~AbstractBehaviour() {
        // empty
    }

    virtual bool update(Time dt ) {
        return onUpdate(dt);
    }

    virtual bool mouseDown() {
        return onMouseDown();
    }

    virtual bool mouseUp() {
        return onMouseUp();
    }

    virtual bool keyboardDown() {
        return onKeyboardDown();
    }

    virtual bool keyboardUp() {
        return onKeyboardUp();
    }

protected:
    AbstractBehaviour() {
        // empty
    }

    virtual bool onUpdate(Time dt) {
        return true;
    }

    virtual bool onMouseDown() {
        return true;
    }

    virtual bool onMouseUp() {
        return true;
    }

    virtual bool onKeyboardDown() {
        return true;
    }

    virtual bool onKeyboardUp() {
        return true;
    }
};

} // Namespace App
} // Namespace OSRE
