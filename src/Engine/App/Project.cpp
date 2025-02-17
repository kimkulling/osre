/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "App/Project.h"
#include "App/Scene.h"
#include "App/Entity.h"
#include "App/TransformComponent.h"
#include "Common/Logger.h"
#include "Common/TAABB.h"
#include "Debugging/osre_debugging.h"
#include "Properties/Property.h"
#include "IO/Uri.h"
#include "RenderBackend/Mesh.h"

namespace OSRE {
namespace App {

using namespace ::OSRE::IO;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

static constexpr c8 Tag[] = "Project";
static constexpr ui32 MajorProjectVerion = 0;
static constexpr ui32 MinorProjectVerion = 1;
static constexpr c8 Ext[] = "osre";

Project::Project() :
        Object("App/Project"),
        mProjectDirtyState(0),
        mProjectName() {
    // empty
}

const c8 *Project::getExtension() {
    return Ext;
}

void Project::setProjectName(const String &projectName) {
    if (mProjectName != projectName) {
        mProjectName = projectName;
    }
}

const String &Project::getProjectName() const {
    return mProjectName;
}

void Project::clear() {
}

bool Project::load(const String &name) {
    if (name.empty()) {
        osre_warn(Tag, "Project name is empty.");
        return false;
    }

    mProjectName = name;

    return true;
}

static void setNameChunk(const String &name, ChunkName &cn) {
    if (name.empty()) {
        cn.mLenChunkName = 0;
        cn.mChunkName = nullptr;
        return;
    }

    cn.mLenChunkName = (i32) name.size();
    cn.mChunkName = new c8[cn.mLenChunkName];
    ::memcpy(cn.mChunkName, name.c_str(), cn.mLenChunkName);
}
    
static size_t getNumNodes(TransformComponent *node, size_t currentNodeCount) {
    if (node->getNumChildren() == 0) {
        return currentNodeCount;
    }
    size_t numChildren = node->getNumChildren();
    currentNodeCount += numChildren;
    for (size_t i = 0; i < numChildren; ++i) {
        TransformComponent *child = node->getChildAt(i);        
        if (child != nullptr) {
            currentNodeCount = getNumNodes(child, currentNodeCount);
        }
    }

    return currentNodeCount;
}
 
static void storeNodes(TransformComponent *currentNode, NodeData *nd, size_t &index) {
    if (currentNode == nullptr || nd == nullptr) {
        return;
    }

    NodeData &curNodeData = nd[index];    
    setNameChunk(currentNode->getName(), curNodeData.mNodeName);
    ::cppcore::TArray<Properties::Property *> propArray;
 
    curNodeData.mNumChildren = (i32) currentNode->getNumChildren();
    curNodeData.mChildrenIndices = new i32[curNodeData.mNumChildren];
    size_t current_child = 0;
    for (int i = 0; i < curNodeData.mNumChildren; i++) {
        TransformComponent *child = currentNode->getChildAt(i);
        curNodeData.mChildrenIndices[current_child] = (i32) i;
        current_child++;
        ++index;
        storeNodes(child, nd, index);
    }
}

static void storeMeshes(MeshArray &meshes, MeshData *md) {
    osre_assert(md != nullptr);
    if (meshes.isEmpty()) {
        return;
    }
    
    if (md == nullptr) {
        return;
    }

    for (size_t i = 0; i < meshes.size(); ++i) {
        Mesh *mesh = meshes[i];
        if (mesh == nullptr) {
            continue;
        }
        setNameChunk(mesh->getName(), md->mMeshName);
        
        // Todo!
    }
}

static void storeEntities(const cppcore::TArray<Entity *> &entities, WorldData &wd) {
    if (entities.isEmpty()) {
        return;
    }
    
    size_t numMeshes = 0;
    wd.mNumEntities = (i32)entities.size();
    wd.mEntityData = new EntityData[wd.mNumEntities];
    for (size_t i = 0; i < entities.size(); ++i) {
        const Entity *entity = entities[i];
        if (entity == nullptr) {
            continue;
        }

        auto *rc = (RenderComponent *)entity->getComponent(ComponentType::RenderComponentType);
        numMeshes += rc->getNumMeshes();
    }

    MeshData *md = new MeshData[numMeshes];
    for (size_t i = 0; i < entities.size(); ++i) {
        const Entity *entity = entities[i];
        if (entity == nullptr) {
            continue;
        }

        auto *rc = (RenderComponent *)entity->getComponent(ComponentType::RenderComponentType);
        MeshArray ma;
        rc->getMeshArray(ma);
        storeMeshes(ma, md);
    }
}

static bool saveScene(const Scene *scene, WorldData &wd) {
    setNameChunk(scene->getName(), wd.mWorldName);

    TransformComponent *root = scene->getRootNode();
    if (nullptr == root) {
        return true;
    }

    size_t numNodes = 1;
    wd.mNumNodes = (i32) getNumNodes(root, numNodes);
    wd.mNodes = new NodeData[wd.mNumNodes];
    size_t index = 0;
    storeNodes(root, wd.mNodes, index);
    const cppcore::TArray<Entity *> &entities = scene->getEntityArray();
    storeEntities(entities, wd);

    return true;
}

bool Project::save(const String &name) {
    if (name.empty()) {
        return false;
    }

        
    return true;
}

} // Namespace App
} // Namespace OSRE
