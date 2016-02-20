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

#include <cppcore/Container/TArray.h>

namespace OSRE {
    
namespace RenderBackend {
    class RenderBackendService;
    
    struct StaticGeometry;
    struct TransformBlock;
}

namespace Scene {

class Component {
public:
    virtual ~Component();
    virtual void update( RenderBackend::RenderBackendService *renderBackendSrv ) = 0;

protected:
    Component();
};

class RenderComponent : public Component {
public:
    RenderComponent();
    virtual ~RenderComponent();
    virtual void update( RenderBackend::RenderBackendService *renderBackendSrv );

    void addStaticGeometry( RenderBackend::StaticGeometry *geo );

private:
    CPPCore::TArray<RenderBackend::StaticGeometry*> m_newGeo;
};

class TransformComponent : public Component {
public:
    TransformComponent();
    virtual ~TransformComponent();
    virtual void update( RenderBackend::RenderBackendService *renderBackendSrv );

    void setTransformBlock( RenderBackend::TransformBlock *localTransform );

private:
    RenderBackend::TransformBlock *m_localTransform;
};

} // Namespace Scene
} // namespace OSRE
