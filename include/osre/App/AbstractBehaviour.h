/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
-----------------------------------------------------------------------------------------------*/
#pragma once

#include <osre/Common/osre_common.h>

namespace OSRE {
namespace App {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
/// @brief
//-------------------------------------------------------------------------------------------------
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

    virtual bool onUpdate(Time ) {
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
