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

#include <osre/App/AssetsCommon.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Common/Ids.h>
#include <osre/Collision/TAABB.h>

#include <cppcore/Container/TArray.h>

#include <map>

// Forward declarations ---------------------------------------------------------------------------
struct aiScene;
struct aiMesh;
struct aiNode;
struct aiMaterial;
struct aiAnimation;

namespace OSRE {

// Froward declarations
namespace Common {
    class Ids;
}

namespace RenderBackend {
    class Mesh;

    struct UniformVar;
    struct Material;
}

namespace IO {
    class Uri;
}

namespace Scene {
    class Node;
}
    
namespace App {

class Entity;
struct BoneInfo;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AssimpWrapper {
public:
    AssimpWrapper( Common::Ids &ids );
    ~AssimpWrapper();
    bool importAsset( const IO::Uri &file, ui32 flags );
    Entity *getEntity() const;

protected:
    Entity *convertScene();
    void importMeshes( aiMesh *mesh );
	void importBones(aiMesh* mesh);
    void impotNode( aiNode *node, Scene::Node *parent );
    void importMaterial( aiMaterial *material );
    void importAnimation( aiAnimation *animation );

private:
	const aiScene *m_scene;
	RenderBackend::MeshArray m_meshArray;
    Entity *m_entity;
    
	using MaterialArray = CPPCore::TArray<RenderBackend::Material*> ;
    MaterialArray m_matArray;
    Scene::Node *m_parent;
    Common::Ids &m_ids;
    RenderBackend::UniformVar *m_mvpParam;
    String m_root;
    String m_absPathWithFile;

	using BoneInfoArray = ::CPPCore::TArray<BoneInfo*>;
	BoneInfoArray m_boneInfoArray;

	using Bone2NodeMap = std::map<const char*, const aiNode*>;
	Bone2NodeMap m_bone2NodeMap;
};

} // Namespace Assets
} // Namespace OSRE
