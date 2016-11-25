/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Object.h>

namespace OSRE {

// Forward declarations
namespace RenderBackend {
    class RenderBackendService;
}

namespace Scene {

class Stage;
class View;

enum class RenderMode {
    Render2D,
    Render3D
};

class OSRE_EXPORT World : public Common::Object {
public:
    World( const String &worldName, RenderMode renderMode = RenderMode::Render3D );
    virtual ~World();
    void addStage( Stage *stage );
    bool setActiveStage( Stage *activeStage );
    bool setActiveStage( const String &stageName );
    void addView( View *view );
    bool setActiveView( View *activeView );
    bool setActiveView( const String &viewName );
    void update( RenderBackend::RenderBackendService *rbService );
    RenderMode getRenderMode() const;

private:
    struct Impl;
    Impl *m_impl;
};

} // Namespace Scene
} // namespace OSRE

