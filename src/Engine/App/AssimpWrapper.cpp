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

struct BoneInfo {
};

AssimpWrapper::AssimpWrapper(Common::Ids &ids, World *world) :
        m_scene(nullptr),
        m_meshArray(),
        mDefaultTexture(nullptr),
        m_entity(nullptr),
        mWorld(world),
        m_matArray(),
        m_parent(nullptr),
        m_ids(ids),
        m_mvpParam(nullptr),
        m_root(),
        m_absPathWithFile(),
        m_boneInfoArray(),
        m_bone2NodeMap() {
    // empty
}

AssimpWrapper::~AssimpWrapper() {
    ::CPPCore::ContainerClear(m_boneInfoArray);

    delete mDefaultTexture;
    mDefaultTexture = nullptr;
}

bool AssimpWrapper::importAsset(const IO::Uri &file, ui32 flags) {
    if (!file.isValid()) {
        osre_error(Tag, "URI " + file.getUri() + " is invalid ");
        return false;
    }

    if (0 == flags) {
        flags = DefaultImportFlags;
    }

    m_root = AssetRegistry::getPath("media");
    m_absPathWithFile = AssetRegistry::resolvePathFromUri(file);

    String filename;
    if (!Directory::getDirectoryAndFile(m_absPathWithFile, m_root, filename)) {
        return false;
    }

    filename = m_root + filename;
    Importer myImporter;
    m_scene = myImporter.ReadFile(filename, flags);
    if (nullptr == m_scene) {
        m_root = "";
        m_absPathWithFile = "";
        return false;
    }
    convertScene();

    return true;
}

Entity *AssimpWrapper::getEntity() const {
    return m_entity;
}

Entity *AssimpWrapper::convertScene() {
    if (nullptr == m_scene) {
        return nullptr;
    }
    if (nullptr == mWorld) {
        mWorld = new World("scene");
    }

    m_entity = new Entity(m_absPathWithFile, m_ids, mWorld);

    if (m_scene->HasMaterials()) {
        for (ui32 i = 0; i < m_scene->mNumMaterials; ++i) {
            aiMaterial *currentMat(m_scene->mMaterials[i]);
            if (nullptr == currentMat) {
                continue;
            }

            importMaterial(currentMat);
        }
    }

    if (m_scene->HasMeshes()) {
        importMeshes(m_scene->mMeshes, m_scene->mNumMeshes);
    }

    if (nullptr != m_scene->mRootNode) {
        importNode(m_scene->mRootNode, nullptr);
    }

    if (nullptr != m_scene->mAnimations) {
        for (ui32 i = 0; i < m_scene->mNumAnimations; ++i) {
            importAnimation(m_scene->mAnimations[i]);
        }
    }

    if (!m_meshArray.isEmpty()) {
        m_entity->addStaticMeshes(m_meshArray);
    }

    return m_entity;
}

static void copyAiMatrix4(const aiMatrix4x4 &aiMat, glm::mat4 &mat) {
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

    TAABB<f32> aabb = m_entity->getAABB();

    Mat2MeshMap mat2MeshMap;
    for (ui32 i = 0; i < numMeshes; ++i) {
        aiMesh *currentMesh = meshes[i];
        if (nullptr == currentMesh) {
            osre_debug(Tag, "Invalid mesh instance found.");
            continue;
        }

        aiMaterial *mat = m_scene->mMaterials[currentMesh->mMaterialIndex];
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
        miArray->add((size_t)i);
    }

    for (size_t i = 0; i < mat2MeshMap.size(); ++i) {
        m_meshArray.add(Mesh::create(1));
    }

    size_t i = 0;
    aiMesh *currentMesh = nullptr;
    ::CPPCore::TArray<RenderVert> vertices;
    for (Mat2MeshMap::iterator it = mat2MeshMap.begin(); it != mat2MeshMap.end(); ++it) {
        CPPCore::TArray<ui32> indexArray;
        MeshIdxArray *miArray = it->second;
        if (nullptr == miArray) {
            continue;
        }

        const size_t numVerts = countVertices(*miArray, m_scene);
        vertices.resize(numVerts);
        Mesh &newMesh = *m_meshArray[i];
        newMesh.m_vertextype = VertexType::RenderVertex;
        size_t vertexOffset = 0, indexOffset = 0;
        for (ui32 j = 0; j < miArray->size(); ++j) {
            const size_t meshIndex = (*miArray)[j];
            currentMesh = m_scene->mMeshes[meshIndex];
            if (nullptr == currentMesh) {
                continue;
            }

            for (ui32 k = 0; k < currentMesh->mNumVertices; ++k) {
                if (currentMesh->HasPositions()) {
                    aiVector3D &vec3 = currentMesh->mVertices[k];
                    vertices[vertexOffset].position.x = vec3.x;
                    vertices[vertexOffset].position.y = vec3.y;
                    vertices[vertexOffset].position.z = vec3.z;

                    aabb.merge(vec3.x, vec3.y, vec3.z);
                }

                if (currentMesh->HasNormals()) {
                    aiVector3D &normal = currentMesh->mNormals[k];
                    vertices[vertexOffset].normal.x = normal.x;
                    vertices[vertexOffset].normal.y = normal.y;
                    vertices[vertexOffset].normal.z = normal.z;
                }

                if (currentMesh->HasVertexColors(0)) {
                    aiColor4D &diffuse = currentMesh->mColors[0][k];
                    vertices[vertexOffset].color0.r = diffuse.r;
                    vertices[vertexOffset].color0.g = diffuse.g;
                    vertices[vertexOffset].color0.b = diffuse.b;
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
                    for (ui32 l = 0; l < currentMesh->mNumBones; ++l) {
                        aiBone *currentBone(currentMesh->mBones[l]);
                        if (nullptr == currentBone) {
                            osre_debug(Tag, "Invalid bone instance found.");
                            continue;
                        }

                        Bone *bone = new Bone;
                        bone->m_name = currentBone->mName.C_Str();
                        copyAiMatrix4(currentBone->mOffsetMatrix, bone->m_offsetMatrix);
                        for (ui32 weightIdx = 0; weightIdx < currentBone->mNumWeights; ++weightIdx) {
                            aiVertexWeight &aiVW = currentBone->mWeights[l];
                            VertexWeight *w = new VertexWeight;
                            w->m_vertexIdx = aiVW.mVertexId;
                            w->m_vertexWeight = aiVW.mWeight;
                            bone->m_vertexWeights.add(w);
                        }
                        const aiNode *node = m_scene->mRootNode->FindNode(bone->m_name.c_str());
                        if (nullptr != node) {
                            m_bone2NodeMap[bone->m_name.c_str()] = node;
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

            const size_t vbSize(sizeof(RenderVert) * numVerts);
            newMesh.m_vb = BufferData::alloc(BufferType::VertexBuffer, vbSize, BufferAccessType::ReadOnly);
            newMesh.m_vb->copyFrom(&vertices[0], vbSize);

            newMesh.m_ib = BufferData::alloc(BufferType::IndexBuffer, sizeof(ui32) * indexArray.size(), BufferAccessType::ReadOnly);
            newMesh.m_ib->copyFrom(&indexArray[0], newMesh.m_ib->getSize());

            //            Debugging::MeshDiagnostic::dumpIndices( indexArray );

            newMesh.createPrimitiveGroup(IndexType::UnsignedInt, indexArray.size(), PrimitiveType::TriangleList, 0);

            const size_t matIdx(currentMesh->mMaterialIndex);
            Material *osreMat = m_matArray[matIdx];
            newMesh.m_material = osreMat;
        }

        ++i;
    }
    m_entity->setAABB(aabb);

    for (Mat2MeshMap::iterator it = mat2MeshMap.begin(); it != mat2MeshMap.end(); ++it) {
        delete it->second;
    }
    mat2MeshMap.clear();

    //GeometryDiagnosticUtils::dumVertices( vertices, numVertices );
}

void AssimpWrapper::importNode(aiNode *node, Scene::Node *parent) {
    if (nullptr == node) {
        return;
    }

    Node *newNode = new Node(node->mName.C_Str(), m_ids, parent);

    // If this is the root-node of the model, set it as the root for the model
    if (nullptr == m_parent) {
        m_parent = newNode;
        m_entity->setNode(newNode);
    }

    if (node->mNumMeshes > 0) {
        for (ui32 i = 0; i < node->mNumMeshes; i++) {
            const ui32 meshIdx(node->mMeshes[i]);
            if (meshIdx >= m_meshArray.size()) {
                continue;
            }

            Mesh *geo(m_meshArray[meshIdx]);
            if (nullptr != geo) {
                newNode->addMeshReference(meshIdx);
            }
        }
    }

    for (ui32 i = 0; i < node->mNumChildren; i++) {
        aiNode *currentNode = node->mChildren[i];
        if (nullptr == currentNode) {
            continue;
        }

        importNode(currentNode, newNode);
    }
}

static void setColor4(const aiColor4D &aiCol, Color4 &col) {
    col.m_r = aiCol.r;
    col.m_g = aiCol.g;
    col.m_b = aiCol.b;
    col.m_a = aiCol.a;
}

using IO::AbstractFileSystem;
using IO::Stream;

static void setTexture(const String &resolvedPath, const aiString &texPath, TextureResourceArray &texResArray, TextureStageType stage) {
    String texname;
    texname += "file://";
    texname += resolvedPath;
    String temp(texPath.C_Str()), temp1;
    IO::Uri::normalizePath(temp, '\\', temp1);
    String::size_type pos = temp1.find("./");
    if (String::npos != pos) {
        temp1 = temp1.substr(pos + 2, temp1.size() + pos + 2);
    }
    texname += temp1;

    TextureResource *texRes = new TextureResource(texname, IO::Uri(texname));
    texRes->setTextureStage(stage);
    texResArray.add(texRes);
}

void AssimpWrapper::importMaterial(aiMaterial *material) {
    if (nullptr == material) {
        return;
    }

    i32 texIndex(0);
    aiString texPath; // contains filename of texture
    TextureResourceArray texResArray;
    if (AI_SUCCESS == material->GetTexture(aiTextureType_DIFFUSE, texIndex, &texPath)) {
        setTexture(m_root, texPath, texResArray, TextureStageType::TextureStage0);
    } 

    String matName = texPath.C_Str();
    if (matName.empty()) {
        matName = "material1";
    }

    Material *osreMat = MaterialBuilder::createTexturedMaterial(matName, texResArray, RenderBackend::VertexType::RenderVertex);
    if (nullptr == osreMat) {
        osre_error(Tag, "Err ehilr creating material for " + matName);
        return;
    }

    m_matArray.add(osreMat);

    aiColor4D diffuse;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse)) {
        setColor4(diffuse, osreMat->m_color[(ui32)MaterialColorType::Mat_Diffuse]);
    }

    aiColor4D specular;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular)) {
        setColor4(specular, osreMat->m_color[(ui32)MaterialColorType::Mat_Specular]);
    }

    aiColor4D ambient;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_AMBIENT, &ambient)) {
        setColor4(ambient, osreMat->m_color[(ui32)MaterialColorType::Mat_Ambient]);
    }

    aiColor4D emission;
    if (AI_SUCCESS == aiGetMaterialColor(material, AI_MATKEY_COLOR_EMISSIVE, &emission)) {
        setColor4(emission, osreMat->m_color[(ui32)MaterialColorType::Mat_Emission]);
    }

    ai_real shininess, strength;
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
