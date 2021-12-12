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
#pragma once

#include <osre/Common/Object.h>
#include <osre/Scene/SceneCommon.h>

// Forward declarations ---------------------------------------------------------------------------
namespace OSRE {
namespace App {
	
class World;

class Stage {
public:
    Stage() : mWorlds() {
        // empty
    }

    ~Stage() {
        // empty
    }

    bool isEmpty() const {
        return mWorlds.mCurrentWorld == nullptr;
    }

    World *getCurrentWorld() const {
        return mWorlds.mCurrentWorld;
    }

private:
    struct Worlds {
        World *mCurrentWorld;
        World *mPrev;
        World *mNext;

        Worlds() :
                mCurrentWorld( nullptr ),
                mPrev( nullptr ),
                mNext( nullptr ) {
            // empty
        }
    };
    Worlds mWorlds;
};

constexpr i32 NotInited = -1;

struct Chunk {
    i32 mSize;
    c8 *mData;

    Chunk() : mSize(NotInited), mData(nullptr) {}
};

struct ChunkName {
    i32 mLenChunkName;
    c8 *mChunkName;

    ChunkName() : mLenChunkName(0), mChunkName(nullptr) {}
};

struct Mat4Data {
    Chunk mData;

    Mat4Data() : mData() {}
};

struct NodeData {
    ChunkName mNodeName;
    i32 mNumChildren;
    i32 *mChildrenIndices;
    i32 mModelIndex;
    f32 mAABB[6];
    i32 mNumProperties;
    i32 mPropertyDataSize;
    c8 *mPropertyData;

    NodeData() :
            mNodeName(), mNumChildren(NotInited), mChildrenIndices(nullptr), mModelIndex(NotInited), mNumProperties(0), mPropertyDataSize(0), mPropertyData(nullptr) {
        // empty
    }
};

struct LightData : NodeData {
    LightData() : NodeData() {}
};

struct CameraData : NodeData {
    CameraData() : NodeData() {}
};

struct BufferData {
    i32 mId;
    i32 mType;
    Chunk mBuffer;

    BufferData() : mId(NotInited), mType(NotInited), mBuffer() {}
};

struct MaterialData {
    ChunkName mMaterialName;

    MaterialData() : mMaterialName() {}
};

struct MeshData {
    ChunkName mMeshName;
    i32 mNumBuffer;
    i32 *mBuffers;

    MeshData() :
            mMeshName(), mNumBuffer(NotInited), mBuffers(nullptr) {}
};

struct EntityData {
    ChunkName mEntityName;

    EntityData() :
            mEntityName() {}
};

struct WorldData {
    ChunkName mWorldName;
    i32 mNumNodes;
    NodeData *mNodes;
    i32 mNumMeshes;
    MeshData *mMeshes;
    i32 mNumBuffers;
    BufferData *mBuffers;
    i32 mRootNode;
    i32 mNumEntities;
    EntityData *mEntityData;

    WorldData() :
            mWorldName(), 
            mNumNodes(NotInited), 
            mNodes(nullptr), 
            mNumMeshes(NotInited), 
            mMeshes(nullptr), 
            mNumBuffers(NotInited), 
            mBuffers(nullptr), 
            mRootNode(NotInited),
            mNumEntities(NotInited),
            mEntityData(nullptr) {
        // empty
    }
};

struct StageData {
    i32 mMajorVersion;
    i32 mMinorVersion;
    ChunkName mStageName;
    i32 mNumWorlds;
    WorldData *mWorldData;

    StageData() :
            mMajorVersion(NotInited),
            mMinorVersion(NotInited),
            mStageName(),
            mNumWorlds(NotInited),
            mWorldData(nullptr) {
        // empty
    }
};

class OSRE_EXPORT Project : public Common::Object {
public:
	Project();
	~Project();
    void setProjectName(const String& projectName);
    const String& getProjectName() const;
    bool load(const String& name);
	bool save( const String &name);

private:
    String mProjectName;
    Stage mStage;
};

} // Namespace App
} // Namespace OSRE
