/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "App/World.h"
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
        mProjectName(),
        mStage(nullptr) {
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

void Project::addAsset( const String &assetName ) {
    if (assetName.empty()) {
        return;
    }
    
    StringArray::Iterator it = mAssetArray.find(assetName);
    if (it == mAssetArray.end()) {
        mAssetArray.add(assetName);
    }
}
size_t Project::getNumAssets() const {
    return mAssetArray.size();
}

const String &Project::getAssetAt( size_t index ) const {
    if (index >= mAssetArray.size()) {
        static String none = "none";
        return none;
    }
    
    return mAssetArray[index];
}

bool Project::removeAsset( const String &assetName ) {
    if (assetName.empty()) {
        return false;
    }

    bool found = false;
    for (size_t i = 0; i < mAssetArray.size(); i++) {
        if (mAssetArray[i] == assetName) {
            mAssetArray.remove(i);
            found = true;
            break;
        }
    }
    
    return found;
}

void Project::setStage( Stage *stage ) {
    if (stage != mStage) {
        mStage = stage;
    }
}

Stage *Project::getStage() const {
    return mStage;
}

void Project::clear() {
}

bool Project::load(const String &name, Stage *stage) {
    if (name.empty()) {
        osre_warn(Tag, "Project name is empty.");
        return false;
    }

    if (stage == nullptr) {
        osre_fatal(Tag, "Stage container is a nullptr.");
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

static bool saveWorld(World *world, WorldData &wd) {
    setNameChunk(world->getName(), wd.mWorldName);

    TransformComponent *root = world->getRootNode();
    if (nullptr == root) {
        return true;
    }

    size_t numNodes = 1;
    wd.mNumNodes = (i32) getNumNodes(root, numNodes);
    wd.mNodes = new NodeData[wd.mNumNodes];
    size_t index = 0;
    storeNodes(root, wd.mNodes, index);
    cppcore::TArray<Entity *> entities;
    world->getEntityArray(entities);
    storeEntities(entities, wd);

    return true;
}

static bool saveStage(const String &name, const Stage &stage, StageData *sd) {
    String stageName = name;
    if (name.empty()) {
        stageName = "Empty Stage";
    }

    setNameChunk(stageName, sd->mStageName);
    sd->mMajorVersion = MajorProjectVerion;
    sd->mMinorVersion = MinorProjectVerion;
    const Stage::WorldArray &worlds = stage.getActiveWorlds();
    if (worlds.isEmpty()) {
        return true;
    }

    bool result = true;
    for (size_t i=0; i<worlds.size(); ++i){
        World *activeWorld = worlds[i];
        setNameChunk(activeWorld->getName(), sd->mActiveWorld);
        sd->mNumWorlds = (i32) stage.getNumberOfWorlds();
        sd->mWorldData = new WorldData[sd->mNumWorlds];
        for (i32 j = 0; j < sd->mNumWorlds; ++j) {
            World *world = stage.getWorldAt(j);
            result |= saveWorld(world, *(sd->mWorldData));
            if (!result) {
                osre_error(Tag, "Error while storing world " + world->getName() + ".");
            }
        }
    }
    
    return result;
}

bool Project::save(const String &name, const Stage *stage) {
    if (name.empty()) {
        return false;
    }

    if (stage == nullptr) {
        return true;
    }
    
    StageData sd;
    const bool result = saveStage(name, *stage, &sd);

    return result;
}

} // Namespace App
} // Namespace OSRE
