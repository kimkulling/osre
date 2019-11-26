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
#include <osre/Assets/AssimpWrapper.h>
#include <osre/Assets/Model.h>
#include <osre/IO/Uri.h>
#include <osre/IO/Directory.h>
#include <osre/Common/Logger.h>
#include <osre/Common/Ids.h>
#include <osre/Common/StringUtils.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/Mesh.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/MaterialBuilder.h>
#include <osre/Scene/Component.h>
#include <osre/Scene/Node.h>
#include <osre/Collision/TAABB.h>
#include <osre/IO/IOService.h>
#include <osre/IO/AbstractFileSystem.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/vector3.h>

#include <iostream>

namespace OSRE {
namespace Assets {
    
using namespace ::Assimp;
using namespace ::OSRE::Common;
using namespace ::OSRE::IO;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;
using namespace ::OSRE::Collision;

static const c8* Tag = "AssimpWrapper";

struct BoneInfo {

};

AssimpWrapper::AssimpWrapper( Common::Ids &ids )
: m_scene( nullptr )
, m_meshArray()
, m_matArray()
, m_model( nullptr )
, m_parent( nullptr )
, m_ids( ids )
, m_mvpParam( nullptr )
, m_root()
, m_absPathWithFile()
, m_boneInfoArray()
, m_bone2NodeMap() {
    // empty
}

AssimpWrapper::~AssimpWrapper() {
	::CPPCore::ContainerClear(m_boneInfoArray);
}

bool AssimpWrapper::importAsset( const IO::Uri &file, ui32 flags ) {
    if ( !file.isValid() ) {
        osre_error( Tag, "URI " + file.getUri() + " is invalid " );
        return false;
    }

    unsigned int importFlags = aiProcess_CalcTangentSpace
        | aiProcess_GenSmoothNormals
        | aiProcess_JoinIdenticalVertices
        | aiProcess_ImproveCacheLocality
        | aiProcess_LimitBoneWeights
        | aiProcess_RemoveRedundantMaterials
        | aiProcess_SplitLargeMeshes
        | aiProcess_Triangulate
        | aiProcess_GenUVCoords
        | aiProcess_SortByPType;
    flags = importFlags;

    m_root = AssetRegistry::getPath( "media" );
    m_absPathWithFile = AssetRegistry::resolvePathFromUri( file );

    String filename;
    Directory::getDirectoryAndFile(m_absPathWithFile, m_root, filename);
    filename = m_root + filename;
    Importer myImporter;
    m_scene = myImporter.ReadFile( filename, flags );
    if ( nullptr == m_scene) {
        m_root = "";
        m_absPathWithFile = "";
        return false;
    }
    convertSceneToModel();
    m_model->setMeshArray( m_meshArray );

    return true;
}

Model *AssimpWrapper::getModel() const {
    return m_model;
}

Model *AssimpWrapper::convertSceneToModel() {
    if ( nullptr == m_scene) {
        return nullptr;
    }

    m_model = new Model;
    if (m_scene->HasMaterials() ) {
        for ( ui32 i = 0; i < m_scene->mNumMaterials; ++i ) {
            aiMaterial *currentMat(m_scene->mMaterials[ i ] );
            if (nullptr == currentMat) {
                continue;
            }
            
            importMaterial(currentMat);
        }
    }
    
    if (m_scene->HasMeshes() ) {
        for ( ui32 i = 0; i < m_scene->mNumMeshes; ++i ) {
            aiMesh *currentMesh( m_scene->mMeshes[ i ] );
            if ( nullptr == currentMesh ) {
                continue;
            }
            importMeshes( currentMesh );
        }
    }

    if ( nullptr != m_scene->mRootNode ) {
        impotNode(m_scene->mRootNode, nullptr );
    }

    if (nullptr != m_scene->mAnimations) {
        for (ui32 i = 0 ; i < m_scene->mNumAnimations; ++i) {
            importAnimation(m_scene->mAnimations[i]);
        }
    }

    return m_model;
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

void AssimpWrapper::importMeshes( aiMesh *mesh ) {
    if ( nullptr == mesh ) {
        return;
    }

    TAABB<f32> aabb = m_model->getAABB();
    Mesh *currentMesh( Mesh::create( 1 ) );
	currentMesh->m_vertextype = VertexType::RenderVertex;
    ui32 numVertices( mesh->mNumVertices );
    RenderVert *vertices = new RenderVert[ numVertices ];
    for ( ui32 i = 0; i < numVertices; i++ ) {
        if ( mesh->HasPositions() ) {
            aiVector3D &vec3 = mesh->mVertices[ i ];
            vertices[ i ].position.x = vec3.x;
            vertices[ i ].position.y = vec3.y;
            vertices[ i ].position.z = vec3.z;

            aabb.merge( vec3.x, vec3.y, vec3.z );
        }

        if ( mesh->HasNormals() ) {
            aiVector3D &normal = mesh->mNormals[ i ];
            vertices[ i ].normal.x = normal.x;
            vertices[ i ].normal.y = normal.y;
            vertices[ i ].normal.z = normal.z;
        }

        if ( mesh->HasVertexColors( 0 ) ) {
            aiColor4D &diffuse = mesh->mColors[ 0 ][ i ];
            vertices[ i ].color0.r = diffuse.r;
            vertices[ i ].color0.g = diffuse.g;
            vertices[ i ].color0.b = diffuse.b;
        }

        for ( ui32 texIdx =0; texIdx<AI_MAX_NUMBER_OF_TEXTURECOORDS; texIdx++ ) {
            if ( mesh->HasTextureCoords( texIdx ) ) {
                if ( 0 == texIdx ) {
                    aiVector3D &tex0 = mesh->mTextureCoords[ texIdx ][ i ];
                    vertices[ i ].tex0.x = tex0.x;
                    vertices[ i ].tex0.y = tex0.y;
                }
            }
        }
    }
    //GeometryDiagnosticUtils::dumVertices( vertices, numVertices );

    if (mesh->HasBones()) {
        for (ui32 i = 0; i < mesh->mNumBones; ++i) {
            aiBone *currentBone(mesh->mBones[i]);
            if (nullptr == currentBone) {
                continue;
            }

            Bone *bone = new Bone;
            bone->m_name = currentBone->mName.C_Str();
            copyAiMatrix4(currentBone->mOffsetMatrix, bone->m_offsetMatrix);
            for (ui32 weightIdx = 0; weightIdx < currentBone->mNumWeights; ++weightIdx) {
                aiVertexWeight &aiVW = currentBone->mWeights[i];
                VertexWeight *w = new VertexWeight;
                w->m_vertexIdx = aiVW.mVertexId;
                w->m_vertexWeight = aiVW.mWeight;
                bone->m_vertexWeights.add(w);
            }
			const aiNode* node = m_scene->mRootNode->FindNode(bone->m_name.c_str());
			if (nullptr != node) {
				m_bone2NodeMap[bone->m_name.c_str() ]= node;
			}
        }
    }

    const ui32 matIdx( mesh->mMaterialIndex );
    Material *osreMat = m_matArray[ matIdx ];
    currentMesh->m_material = osreMat;
    const ui32 vbSize( sizeof( RenderVert ) * numVertices );
    currentMesh->m_vb = BufferData::alloc( BufferType::VertexBuffer, vbSize, BufferAccessType::ReadOnly );
    currentMesh->m_vb->copyFrom( &vertices[ 0 ], vbSize );

    CPPCore::TArray<ui32> indexArray;
    for ( ui32 i = 0; i < mesh->mNumFaces; i++ ) {
        aiFace &currentFace = mesh->mFaces[ i ];
        for ( ui32 idx = 0; idx < currentFace.mNumIndices; idx++ ) {
            const ui32 index = currentFace.mIndices[ idx ];
            indexArray.add( index );
        }
    }
	//Scene::GeometryDiagnosticUtils::dumpIndices( indexArray );

    currentMesh->m_ib = BufferData::alloc( BufferType::IndexBuffer, sizeof( ui32 ) * indexArray.size(), BufferAccessType::ReadOnly );
    currentMesh->m_ib->copyFrom( &indexArray[ 0 ], currentMesh->m_ib->getSize());

    currentMesh->createPrimitiveGroup(IndexType::UnsignedInt, indexArray.size(), PrimitiveType::TriangleList, 0);
    currentMesh->m_material = m_matArray[matIdx];
    
    m_meshArray.add( currentMesh );
    m_model->setAABB( aabb );
}

void AssimpWrapper::impotNode( aiNode *node, Scene::Node *parent ) {
    if ( nullptr == node) {
        return;
    }
    
    Node *newNode = new Node( node->mName.C_Str(), m_ids, parent );
    
    // If this is the root-node of the model, set it as the root for the model
    if ( nullptr == m_parent ) {
        m_parent = newNode;
        m_model->setRootNode( m_parent );
    }

    if ( node->mNumMeshes > 0 ) {
        for ( ui32 i = 0; i < node->mNumMeshes; i++ ) {
            const ui32 meshIdx( node->mMeshes[ i ] );
            if ( meshIdx >= m_meshArray.size() ) {
                continue;
            }

            Mesh *geo( m_meshArray[ meshIdx ] );
            if ( nullptr != geo ) {
                newNode->addMesh( geo );
            }
        }
    }

    for ( ui32 i = 0; i < node->mNumChildren; i++ ) {
        aiNode *currentNode = node->mChildren[ i ];
        if ( nullptr == currentNode) {
            continue;
        }

        impotNode( currentNode, newNode );
    }
}

static void setColor4( const aiColor4D &aiCol, Color4 &col ) {
    col.m_r = aiCol.r;
    col.m_g = aiCol.g;
    col.m_b = aiCol.b;
    col.m_a = aiCol.a;
}

using IO::Stream;
using IO::AbstractFileSystem;

static void setTexture( const String &resolvedPath, const aiString &texPath, TextureResourceArray &texResArray, TextureStageType stage ) {
    String texname;
    texname += "file://";
    texname += resolvedPath;
    String temp( texPath.C_Str() ), temp1;
    IO::Uri::normalizePath( temp, '\\', temp1 );
    String::size_type pos = temp1.find( "./" );
    if (String::npos != pos) {
        temp1 = temp1.substr( pos + 2, temp1.size() + pos + 2 );
    }
    texname += temp1;

    TextureResource *texRes = new TextureResource(texname, IO::Uri(texname) );
    texRes->setTextureStage( stage );
    texResArray.add(texRes);
}

void AssimpWrapper::importMaterial( aiMaterial *material ) {
    if ( nullptr == material ) {
        return;
    }
    
    i32 texIndex( 0 );
    aiString texPath;	// contains filename of texture
    TextureResourceArray texResArray;
    if ( AI_SUCCESS == material->GetTexture( aiTextureType_DIFFUSE, texIndex, &texPath ) ) {
        setTexture(m_root, texPath, texResArray, TextureStageType::TextureStage0 );
    }
    String matName = texPath.C_Str();
    if (matName.empty()) {
        matName = "material1";
    }
    Material* osreMat = MaterialBuilder::createTexturedMaterial( matName, texResArray, RenderBackend::VertexType::RenderVertex);
    if (nullptr == osreMat) {
        osre_error( Tag, "Err ehilr creating material for " + matName );
        return;
    }

    m_matArray.add(osreMat);

    aiColor4D diffuse;
    if ( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_DIFFUSE, &diffuse ) ) {
        setColor4( diffuse, osreMat->m_color[ (ui32) MaterialColorType::Mat_Diffuse ] );
    }

    aiColor4D specular;
    if ( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_SPECULAR, &specular ) ) {
        setColor4( specular, osreMat->m_color[ ( ui32 ) MaterialColorType::Mat_Specular ] );
    }

    aiColor4D ambient;
    if ( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_AMBIENT, &ambient ) ) {
        setColor4( ambient, osreMat->m_color[ ( ui32 ) MaterialColorType::Mat_Ambient ] );
    }

    aiColor4D emission;
    if ( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_EMISSIVE, &emission ) ) {
        setColor4( emission, osreMat->m_color[ ( ui32 ) MaterialColorType::Mat_Emission ] );
    }

    ai_real shininess, strength;
    unsigned int max;	// changed: to unsigned
    if ( AI_SUCCESS == aiGetMaterialFloatArray( material, AI_MATKEY_SHININESS, &shininess, &max ) ) {
        // todo
    }

    if ( AI_SUCCESS == aiGetMaterialFloatArray( material, AI_MATKEY_SHININESS_STRENGTH, &strength, &max ) ) {
        // todo
    }
}

void AssimpWrapper::importAnimation(aiAnimation *animation) {
    if (nullptr == animation) {
        return;
    }



    
}

} // Namespace Assets
} // Namespace OSRE
