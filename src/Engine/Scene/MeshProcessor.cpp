/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Debugging/osre_debugging.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/MeshProcessor.h>

namespace OSRE {
namespace Scene {

using namespace ::OSRE::RenderBackend;

static const i32 NeedsUpdate = 1;

MeshProcessor::MeshProcessor() :
        AbstractProcessor(),
        mMeshArray(),
        mAabb(),
        mDirty(0) {
    // empty
}

MeshProcessor::~MeshProcessor() {
    // empty
}

bool MeshProcessor::execute() {
    if (mMeshArray.isEmpty()) {
        return true;
    }

    if (mDirty &= NeedsUpdate) {
        for (ui32 i = 0; i < mMeshArray.size(); i++) {
            handleMesh(mMeshArray[i]);
        }
    }

    mDirty = 0;

    return true;
}

void MeshProcessor::addMesh(Mesh *mesh) {
    if (nullptr == mesh) {
        return;
    }

    mMeshArray.add(mesh);

    mDirty |= NeedsUpdate;
}

const Scene::Node::AABB &MeshProcessor::getAABB() const {
    return mAabb;
}

void MeshProcessor::handleMesh(Mesh *mesh) {
    if (nullptr == mesh) {
        return;
    }

    ui32 stride(0);
    switch (mesh->getVertexcType()) {
        case VertexType::RenderVertex:
            stride = sizeof(RenderVert);
            break;

        case VertexType::ColorVertex:
            stride = sizeof(ColorVert);
            break;

        default:
            break;
    }

    BufferData *data = mesh->getVertexBuffer();
    if (nullptr == data || 0L == data->getSize()) {
        return;
    }

    ui32 offset = 0;
    const ui32 numVertices = (ui32)data->getSize() / stride;
    for (ui32 i = 0; i < numVertices; i++) {
        glm::vec3 pos = {};
        uc8 *ptr = (uc8*) data->getData();
        ::memcpy(&pos.x, &ptr[offset], sizeof(glm::vec3));
        offset += stride;
        mAabb.merge(pos.x, pos.y, pos.z);
    }
}

} // namespace Scene
} // Namespace OSRE
