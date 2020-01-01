#pragma once

namespace OSRE {
namespace App {

class AbstractBehaviour {
public:
    virtual ~AbstractBehaviour() {
        // empty
    }

    virtual bool update() {
        return onUpdate();
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

    virtual bool onUpdate() {
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
