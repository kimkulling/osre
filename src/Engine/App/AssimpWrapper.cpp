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
#include <osre/App/AssetRegistry.h>
#include <osre/App/AssimpWrapper.h>
#include <osre/App/Component.h>
#include <osre/App/Entity.h>
#include <osre/App/World.h>
#include <osre/Common/Ids.h>
#include <osre/Common/Logger.h>
#include <osre/Common/StringUtils.h>
#include <osre/Debugging/MeshDiagnostic.h>
#include <osre/IO/AbstractFileSystem.h>
#include <osre/IO/Directory.h>
#include <osre/IO/IOService.h>
#include <osre/IO/Uri.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/MaterialBuilder.h>
#include <osre/Scene/MeshBuilder.h>
#include <osre/Scene/MeshProcessor.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/TAABB.h>

#include <assimp/postprocess.h>
#include <assimp/scene.h>
#include <assimp/vector3.h>
#include <assimp/Importer.hpp>

#include <iostream>

namespace OSRE {
namespace App {

using namespace ::Assimp;
using namespace ::OSRE::Common;
using namespace ::OSRE::IO;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;

static const c8 *Tag = "AssimpWrapper";

const unsigned int DefaultImportFlags = aiProcess_CalcTangentSpace | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices |
                                        aiProcess_ImproveCacheLocality | aiProcess_LimitBoneWeights | aiProcess_RemoveRedundantMaterials |
                                        aiProcess_SplitLargeMeshes | aiProcess_Triangulate | aiProcess_GenUVCoords | aiProcess_SortByPType;

static void setColor4(const aiColor4D &aiCol, Color4 &col) {
    col.m_r = aiCol.r;
    col.m_g = aiCol.g;
    col.m_b = aiCol.b;
    col.m_a = aiCol.a;
}

static void setTexture(const String &resolvedPath, const aiString &texPath, 
        TextureResourceArray &texResArray, TextureStageType stage) {
    if (resolvedPath[0] == '*') {
        
    }
    
    String texname;
    texname += "file://";
    texname += resolvedPath;
    String temp = texPath.C_Str(), temp1;
    IO::Uri::normalizePath(temp, '\\', temp1);
    String::size_type pos = temp1.find("./");
    if (String::npos != pos) {
        temp1 = temp1.substr(pos + 2, temp1.size() + pos + 2);
    }
    texname += temp1;

    TextureResource *texRes = new TextureResource(texname, Uri(texname));
    texRes->setTextureStage(stage);
    texResArray.add(texRes);
}

AssimpWrapper::AssetContext::AssetContext(Common::Ids &ids, World *world) :
        mScene(nullptr),
        mMeshArray(),
        mDefaultTexture(nullptr),
        mEntity(nullptr),
        mWorld(world),
        mMatArray(),
        mParentNode(nullptr),
        mIds(ids),
        mRoot(),
        mAbsPathWithFile(),
        mBone2NodeMap() {
    // empty
}
 
AssimpWrapper::AssetContext::~AssetContext() {
    delete mDefaultTexture;
    mDefaultTexture = nullptr;
}

AssimpWrapper::AssimpWrapper( Common::Ids &ids, World *world ) :
        mAssetContext(ids, world) {
    // empty
}

AssimpWrapper::~AssimpWrapper() {
    // empty
}

bool AssimpWrapper::importAsset(const IO::Uri &file, ui32 flags) {
    if (!file.isValid()) {
        osre_error(Tag, "URI " + file.getUri() + " is invalid ");
        return false;
    }

    if (0 == flags) {
        flags = DefaultImportFlags;
    }

    mAssetContext.mRoot = AssetRegistry::getPath("media");
    mAssetContext.mAbsPathWithFile = AssetRegistry::resolvePathFromUri(file);

    String filename;
    if (!Directory::getDirectoryAndFile(mAssetContext.mAbsPathWithFile, mAssetContext.mRoot, filename)) {
        return false;
    }

    filename = mAssetContext.mRoot + filename;
    Importer myImporter;
    osre_debug(Tag, "Start importing " + filename);
    mAssetContext.mScene = myImporter.ReadFile(filename, flags);
    if (nullptr == mAssetContext.mScene) {
        osre_error(Tag, "Start importing " + filename);
        mAssetContext.mRoot = "";
        mAssetContext.mAbsPathWithFile = "";
        return false;
    }
    osre_debug(Tag, "Importing " + filename + " finished.");
    convertScene();
    osre_debug(Tag, "Converting " + filename + " finished.");
    
    return true;
}

Entity *AssimpWrapper::getEntity() const {
    return mAssetContext.mEntity;
}

Entity *AssimpWrapper::convertScene() {
    if (nullptr == mAssetContext.mScene) {
        return nullptr;
    }

    if (nullptr == mAssetContext.mWorld) {
        mAssetContext.mWorld = new World("scene");
    }

    mAssetContext.mEntity = new Entity(mAssetContext.mAbsPathWithFile, mAssetContext.mIds, mAssetContext.mWorld);
    if (mAssetContext.mScene->HasMaterials()) {
        for (ui32 i = 0; i < mAssetContext.mScene->mNumMaterials; ++i) {
            aiMaterial *currentMat = mAssetContext.mScene->mMaterials[i];
            if (nullptr == currentMat) {
                continue;
            }

            importMaterial(currentMat);
        }
    }

    if (mAssetContext.mScene->HasMeshes()) {
        importMeshes(mAssetContext.mScene->mMeshes, mAssetContext.mScene->mNumMeshes);
    }

    if (nullptr != mAssetContext.mScene->mRootNode) {
        importNode(mAssetContext.mScene->mRootNode, nullptr);
    }

    if (nullptr != mAssetContext.mScene->mAnimations) {
        for (ui32 i = 0; i < mAssetContext.mScene->mNumAnimations; ++i) {
            importAnimation(mAssetContext.mScene->mAnimations[i]);
        }
    }

    if (!mAssetContext.mMeshArray.isEmpty()) {
        RenderComponent *rc = (RenderComponent*) mAssetContext.mEntity->getComponent(ComponentType::RenderComponentType);
        rc->addStaticMeshArray(mAssetContext.mMeshArray);
    }

    return mAssetContext.mEntity;
}

static void copyAiMatrix4x4(const aiMatrix4x4 &aiMat, glm::mat4 &mat) {
    mat[0].x = aiMat.a1;
    mat[0].y = aiMat.a2;
    mat[0].z = aiMat.a3;
    mat[0].w = aiMat.a4;

    mat[1].x = aiMat.b1;
    mat[1].y = aiMat.b2;
    mat[1].z = aiMat.b3;
    mat[1].w = aiMat.b4;

    mat[2].x = aiMat.c1;
    mat[2].y = aiMat.c2;
    mat[2].z = aiMat.c3;
    mat[2].w = aiMat.c4;

    mat[3].x = aiMat.d1;
    mat[3].y = aiMat.d2;
    mat[3].z = aiMat.d3;
    mat[3].w = aiMat.d4;
}

using MeshIdxArray = ::CPPCore::TArray<size_t>;
using Mat2MeshMap = std::map<aiMaterial *, MeshIdxArray *>;

static size_t countVertices(MeshIdxArray &miArray, const aiScene *scene) {
    if (nullptr == scene) {
        return 0;
    }

    size_t numVertices = 0;
    for (unsigned int i = 0; i < miArray.size(); ++i) {
        numVertices += static_cast<size_t>(scene->mMeshes[miArray[i]]->mNumVertices);
    }

    return numVertices;
}

void AssimpWrapper::importMeshes(aiMesh **meshes, ui32 numMeshes) {
    if (nullptr == meshes || 0 == numMeshes) {
        osre_debug(Tag, "No meshes, aborting.");
        return;
    }

    TAABB<f32> aabb = mAssetContext.mEntity->getAABB();

    Mat2MeshMap mat2MeshMap;
    for (ui32 meshIndex = 0; meshIndex < numMeshes; ++meshIndex) {
        aiMesh *currentMesh = meshes[meshIndex];
        if (nullptr == currentMesh) {
            osre_debug(Tag, "Invalid mesh instance found.");
            continue;
        }

        aiMaterial *mat = mAssetContext.mScene->mMaterials[currentMesh->mMaterialIndex];
        if (nullptr == mat) {
            continue;
        }
        
        Mat2MeshMap::const_iterator it = mat2MeshMap.find(mat);
        MeshIdxArray *miArray = nullptr;
        if (mat2MeshMap.end() == it) {
            miArray = new MeshIdxArray;
            mat2MeshMap[mat] = miArray;
        } else {
            miArray = it->second;
        }
        miArray->add(static_cast<size_t>(meshIndex));
    }

    for (size_t mat2MeshIdx = 0; mat2MeshIdx < mat2MeshMap.size(); ++mat2MeshIdx) {
        mAssetContext.mMeshArray.add(new Mesh("m1", VertexType::RenderVertex, IndexType::UnsignedInt));
    }

    size_t i = 0;
    aiMesh *currentMesh = nullptr;
    ::CPPCore::TArray<RenderVert> vertices;
    for (auto & it : mat2MeshMap) {
        CPPCore::TArray<ui32> indexArray;
        MeshIdxArray *miArray = it.second;
        if (nullptr == miArray) {
            continue;
        }

        const size_t numVerts = countVertices(*miArray, mAssetContext.mScene);
        vertices.resize(numVerts);
        Mesh &newMesh = *mAssetContext.mMeshArray[i];
        size_t vertexOffset = 0, indexOffset = 0;
        for (unsigned long long meshIndex : *miArray) {
            currentMesh = mAssetContext.mScene->mMeshes[meshIndex];
            if (nullptr == currentMesh) {
                continue;
            }

            for (ui32 k = 0; k < currentMesh->mNumVertices; ++k) {
                if (currentMesh->HasPositions()) {
                    const aiVector3D &vec3 = currentMesh->mVertices[k];
                    vertices[vertexOffset].position.x = vec3.x;
                    vertices[vertexOffset].position.y = vec3.y;
                    vertices[vertexOffset].position.z = vec3.z;

                    aabb.merge(vec3.x, vec3.y, vec3.z);
                }

                if (currentMesh->HasNormals()) {
                    const aiVector3D &normal = currentMesh->mNormals[k];
                    vertices[vertexOffset].normal.x = normal.x;
                    vertices[vertexOffset].normal.y = normal.y;
                    vertices[vertexOffset].normal.z = normal.z;
                }

                if (currentMesh->HasVertexColors(0)) {
                    const aiColor4D &diffuse = currentMesh->mColors[0][k];
                    vertices[vertexOffset].color0.r = diffuse.r;
                    vertices[vertexOffset].color0.g = diffuse.g;
                    vertices[vertexOffset].color0.b = diffuse.b;
                } else {
                    vertices[vertexOffset].color0.r = 0.5;
                    vertices[vertexOffset].color0.g = 0.5;
                    vertices[vertexOffset].color0.b = 0.5;
                }

                for (ui32 texIdx = 0; texIdx < AI_MAX_NUMBER_OF_TEXTURECOORDS; texIdx++) {
                    if (currentMesh->HasTextureCoords(texIdx)) {
                        if (0 == texIdx) {
                            aiVector3D &tex0 = currentMesh->mTextureCoords[texIdx][k];
                            vertices[vertexOffset].tex0.x = tex0.x;
                            vertices[vertexOffset].tex0.y = tex0.y;
                        }
                    }
                }

                if (currentMesh->HasBones()) {
                    Bone *bones = new Bone[currentMesh->mNumBones];
                    for (ui32 l = 0; l < currentMesh->mNumBones; ++l) {
                        aiBone *currentBone = currentMesh->mBones[l];
                        if (nullptr == currentBone) {
                            osre_debug(Tag, "Invalid bone instance found.");
                            continue;
                        }

                        Bone &bone = bones[l];
                        bone.m_name = currentBone->mName.C_Str();
                        copyAiMatrix4x4(currentBone->mOffsetMatrix, bone.m_offsetMatrix);
                        VertexWeight *wArray = new VertexWeight[currentBone->mNumWeights];
                        
                        for (ui32 weightIdx = 0; weightIdx < currentBone->mNumWeights; ++weightIdx) {
                            aiVertexWeight &aiVW = currentBone->mWeights[l];
                            VertexWeight &w = wArray[weightIdx];
                            w.m_vertexIdx = aiVW.mVertexId;
                            w.m_vertexWeight = aiVW.mWeight;
                        }
                        bone.m_vertexWeights.add(wArray, currentBone->mNumWeights);
                        const aiNode *node = mAssetContext.mScene->mRootNode->FindNode(bone.m_name.c_str());
                        if (nullptr != node) {
                            mAssetContext.mBone2NodeMap[bone.m_name.c_str()] = node;
                        }
                    }
                }

                ++vertexOffset;
            }

            for (ui32 faceIdx = 0; faceIdx < currentMesh->mNumFaces; ++faceIdx) {
                aiFace &currentFace = currentMesh->mFaces[faceIdx];
                for (ui32 idx = 0; idx < currentFace.mNumIndices; ++idx) {
                    const ui32 currentIndex = currentFace.mIndices[idx];
                    indexArray.add(static_cast<ui32>(currentIndex + indexOffset));
                }
            }

            indexOffset += currentMesh->mNumVertices;

            const size_t vbSize = sizeof(RenderVert) * numVerts;
            newMesh.createVertexBuffer(&vertices[0], vbSize, BufferAccessType::ReadOnly);

            const size_t ibSize = sizeof(ui32) * indexArray.size();
            newMesh.createIndexBuffer(&indexArray[0], ibSize, IndexType::UnsignedInt, BufferAccessType::ReadOnly);
            //            Debugging::MeshDiagnostic::dumpIndices( indexArray );

            newMesh.addPrimitiveGroup(indexArray.size(), PrimitiveType::TriangleList, 0);

            newMesh.setMaterial(mAssetContext.mMatArray[currentMesh->mMaterialIndex]);
        }

        ++i;
    }
    mAssetContext.mEntity->setAABB(aabb);

    for (auto &it : mat2MeshMap) {
        delete it.second;
    }
    mat2MeshMap.clear();

    //GeometryDiagnosticUtils::dumVertices( vertices, numVertices );
}

void AssimpWrapper::importNode(aiNode *node, Scene::Node *parent) {
    if (nullptr == node) {
        return;
    }

    Node *newNode = new Node(node->mName.C_Str(), mAssetContext.mIds, parent);

    // If this is the root-node of the model, set it as the root for the model
    if (nullptr == mAssetContext.mParentNode) {
        mAssetContext.mParentNode = newNode;
        mAssetContext.mEntity->setNode(newNode);
    }

    if (node->mNumMeshes > 0) {
        for (ui32 i = 0; i < node->mNumMeshes; ++i) {
            const ui32 meshIdx(node->mMeshes[i]);
            if (meshIdx >= mAssetContext.mMeshArray.size()) {
                continue;
            }

            Mesh *mesh = mAssetContext.mMeshArray[meshIdx];
            if (nullptr != mesh) {
                newNode->addMeshReference(meshIdx);
            }
        }
    }

    for (ui32 i = 0; i < node->mNumChildren; ++i) {
        aiNode *currentNode = node->mChildren[i];
        if (nullptr == currentNode) {
            continue;
        }

        importNode(currentNode, newNode);
    }
}

void AssimpWrapper::importMaterial(aiMaterial *material) {
    if (nullptr == material) {
        osre_trace(Tag, "Nullptr for material detected.");
        return;
    }

    i32 texIndex = 0;
    aiString texPath; // contains filename of texture
    TextureResourceArray texResArray;
    if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath)) {
        setTexture(mAssetContext.mRoot, texPath, texResArray, TextureStageType::TextureStage0);
    } 

    String matName = texPath.C_Str();
    if (matName.empty()) {
        matName = "material1";
    }

    Material *osreMat = MaterialBuilder::createTexturedMaterial(matName, texResArray, VertexType::RenderVertex);
    if (nullptr == osreMat) {
        osre_error(Tag, "Error while creating material for " + matName);
        return;
    }

    mAssetContext.mMatArray.add(osreMat);

    const aiColor4D defaultColor(1, 1, 1, 1);
    aiColor4D diffuse = defaultColor;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
        setColor4(diffuse, osreMat->m_color[(ui32)MaterialColorType::Mat_Diffuse]);
    }

    aiColor4D specular = defaultColor;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular)) {
        setColor4(specular, osreMat->m_color[(ui32)MaterialColorType::Mat_Specular]);
    }

    aiColor4D ambient = defaultColor;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)) {
        setColor4(ambient, osreMat->m_color[(ui32)MaterialColorType::Mat_Ambient]);
    }

    aiColor4D emission = defaultColor;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emission)) {
        setColor4(emission, osreMat->m_color[(ui32)MaterialColorType::Mat_Emission]);
    }

    ai_real shininess = 1.0, strength=1.0;
    unsigned int max; // changed: to unsigned
    if (AI_SUCCESS == aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS, &shininess, &max)) {
        osreMat->mShineness = shininess;
    }

    if (AI_SUCCESS == aiGetMaterialFloatArray(material, AI_MATKEY_SHININESS_STRENGTH, &strength, &max)) {
        osreMat->mShinenessStrength = strength;
    }
}

void AssimpWrapper::importAnimation(aiAnimation *animation) {
    if (nullptr == animation) {
        return;
    }
}


} // namespace App
} // Namespace OSRE
