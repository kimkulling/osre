/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Collision/TAABB.h>
#include <cppcore/Container/TArray.h>

// Forward declarations
namespace OSRE {

namespace Scene {
    class Node;
}

namespace Assets {

class OSRE_EXPORT Model {
public: 
    using ModelAABB = Collision::TAABB<f32>;

    Model();
    Model(RenderBackend::MeshArray *geoArray, Scene::Node *root, ModelAABB &aabb );
    ~Model();
    void setMeshArray(RenderBackend::MeshArray &geoArray );
    const RenderBackend::MeshArray &getMeshArray() const;
    void setRootNode( Scene::Node *root );
    Scene::Node *getRootNode() const;
    void setAABB( const Collision::TAABB<f32> &aabb );
    const ModelAABB &getAABB() const;

private:
    RenderBackend::MeshArray *m_meshArray;
    Scene::Node *m_root;
    Collision::TAABB<f32> m_aabb;
};

} // Namespace Assets
} // Namespace OSRE
