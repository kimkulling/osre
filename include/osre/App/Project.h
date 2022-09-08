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
#include <osre/App/Stage.h>

#include <cppcore/Common/TBitField.h>

// Forward declarations ---------------------------------------------------------------------------
namespace OSRE {
namespace App {
	
class World;

constexpr i32 NotInited = -1;
static constexpr ui32 ProjectDirty = 1;

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
    ChunkName mActiveWorld;
    i32 mNumWorlds;
    WorldData *mWorldData;

    StageData() :
            mMajorVersion(NotInited),
            mMinorVersion(NotInited),
            mStageName(),
            mActiveWorld(),
            mNumWorlds(NotInited),
            mWorldData(nullptr) {
        // empty
    }
};

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
/// @brief  This class represents an Osre project.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Project : public Common::Object {
public:
    using ProjectDirtyState = CPPCore::TBitField<ui32>;

    /// @brief The default class constructor.
	Project();

    ///	@brief  The class destructor.
	~Project() override = default;

    /// @brief Will return the common project file extension.
    /// @return The common project file extension.
    static const c8 *getExtension();

    /// @brief Will assign a new project name.
    /// @param[in] projectName  The new project name.
    void setProjectName(const String& projectName);

    /// @brief Will return the assigned project name
    /// @return The assigned project name.
    const String& getProjectName() const;

    /// @brief Will ass a new stored asset name.
    /// @param[in] assetName    The new asset name.
    void addAsset(const String &assetName);
    
    /// @brief Will return the number of assigned assets.
    /// @return The number of assets.
    size_t getNumAssets() const;

    /// @brief Will return the asset name at the given index.
    /// @param[in] index    The index.
    /// @return The assigned name or none.
    const String &getAssetAt(size_t index) const;

    /// @brief Will remove an asset name.
    /// @param[in] assetName    The asset name.
    /// @return true for was removed, false for not.
    bool removeAsset(const String &assetName);
    
    /// @brief Will assign the sate for the project.
    /// @param[in] stage    The stage to use.
    void setStage(Stage *stage);
    
    /// @brief  Will return the assigned stage. Is nullptr if no stage was assigned.
    /// @return The assigned stage or nullptr.
    Stage *getStage() const;
    
    /// @brief Will return true, if the project needs to get saved.
    /// @return true for dirty.
    bool dataNeedsSave() const;

    /// @brief Will clear the project data.
    void clear();

    /// @brief Will load the project file from IO.
    /// @param[in] name     The URI to load from.
    /// @param stage        The stage to load in.
    /// @return true for success, false for failure.
    bool load(const String &name, Stage *stage);

    /// @brief Will save the current project.
    /// @param[in] name     The name for saving.
    /// @param stage        The stage to save.
    /// @return true for success, false for failure.
    bool save(const String &name, const Stage *stage);

private:
    ProjectDirtyState mProjectDirtyState;
    String mProjectName;
    Stage *mStage;
    StringArray mAssetArray;
};

inline bool Project::dataNeedsSave() const {
    return mProjectDirtyState.getBit(ProjectDirty);
}

} // Namespace App
} // Namespace OSRE
