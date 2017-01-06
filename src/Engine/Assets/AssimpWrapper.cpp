/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/Logger.h>
#include <osre/Common/Ids.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Node.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace OSRE {
namespace Assets {
    
using namespace ::Assimp;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;

static const String Tag = "AssimpWrapper";

AssimpWrapper::AssimpWrapper( Common::Ids &ids )
: m_nodeStack()
, m_model( nullptr )
, m_geoArray()
, m_parent( nullptr )
, m_ids( ids ) {
    // empty
}

AssimpWrapper::~AssimpWrapper() {
    // empty
}

bool AssimpWrapper::importAsset( const IO::Uri &file, ui32 flags ) {
    if ( !file.isValid() ) {
        osre_error( Tag, "URI " + file.getUri() + " is invalid " );
        return false;
    }

    String root = AssetRegistry::getPath( "media" );
    String path = AssetRegistry::resolvePathFromUri( file );

    String filename = root + path;
    Importer myImporter;
    const aiScene *scene = myImporter.ReadFile( filename, flags );
    if ( nullptr == scene ) {
        return false;
    }
    convertSceneToModel( scene );

    return true;
}

Model *AssimpWrapper::getModel() const {
    return m_model;
}

Model *AssimpWrapper::convertSceneToModel( const aiScene *scene ) {
    if ( nullptr == scene ) {
        return nullptr;
    }

    m_model = new Model;
    String rootName( scene->mRootNode->mName.C_Str() );
    Node *root = new Node( rootName, m_ids, true, true, nullptr );
    m_parent = root;
    m_model->setRootNode( root );

    if ( scene->HasMaterials() ) {
        for ( ui32 i = 0; i < scene->mNumMaterials; i++ ) {
            aiMaterial *currentMat( scene->mMaterials[ i ] );
            if ( nullptr == currentMat ) {
                continue;
            }
            handleMaterial( currentMat );
        }
    }
    
    if ( scene->HasMeshes() ) {
        for ( ui32 i = 0; i < scene->mNumMeshes; i++ ) {
            aiMesh *currentMesh( scene->mMeshes[ i ] );
            if ( nullptr == currentMesh ) {
                continue;
            }
            handleMesh( currentMesh );
        }
    }

    if ( nullptr != scene->mRootNode ) {
        handleNode( scene->mRootNode );
    }

    return m_model;
}

void AssimpWrapper::handleMesh( aiMesh *mesh ) {
    if ( nullptr == mesh ) {
        return;
    }

    Geometry *geo( Geometry::create( 1 ) );
    ui32 numVertices( mesh->mNumVertices );
    RenderVert *vertices = new RenderVert[ numVertices ];
    for ( ui32 i = 0; i < numVertices; i++ ) {
        if ( mesh->HasPositions() ) {
            aiVector3D &vec3 = mesh->mVertices[ i ];
            vertices[ i ].position.x = vec3.x;
            vertices[ i ].position.y = vec3.y;
            vertices[ i ].position.z = vec3.z;
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

        if ( mesh->HasTextureCoords( 0 ) ) {
            aiVector3D &tex0 = mesh->mTextureCoords[ 0 ][ i ];
            vertices[ i ].tex0.x = tex0.x;
            vertices[ i ].tex0.y = tex0.y;
        }
    }
    const ui32 matIdx( mesh->mMaterialIndex );

    geo->m_vb = BufferData::alloc( BufferType::VertexBuffer, sizeof( RenderVert ) * numVertices, BufferAccessType::ReadOnly );

    CPPCore::TArray<ui16> indexArray;
    for ( ui32 i = 0; i < mesh->mNumFaces; i++ ) {
        aiFace &currentFace = mesh->mFaces[ i ];
        for ( ui32 idx = 0; idx < currentFace.mNumIndices; idx++ ) {
            const ui32 index = currentFace.mIndices[ idx ];
            indexArray.add( static_cast<ui16>( index ) );
        }
    }
    geo->m_ib = BufferData::alloc( BufferType::IndexBuffer, sizeof( ui16 ) * indexArray.size(), BufferAccessType::ReadOnly );
    geo->m_ib->copyFrom( &indexArray[ 0 ], geo->m_ib->m_size );

    m_geoArray.add( geo );
}

void AssimpWrapper::pushNode( Scene::Node *newNode ) {
    if ( nullptr == newNode ) {
        return;
    }
    if ( !m_nodeStack.isEmpty() ) {
        m_parent = m_nodeStack.back();
    } else {
        m_parent = nullptr;
    }
    m_nodeStack.add( newNode );
}

void AssimpWrapper::popNode() {
    m_nodeStack.removeBack();
    if ( !m_nodeStack.isEmpty() ) {
        m_parent = m_nodeStack.back();
    } else {
        m_parent = nullptr;
    }
}

void AssimpWrapper::handleNode( aiNode *node ) {
    if ( nullptr == node ) {
        return;
    }

    const String name( node->mName.C_Str() );
    Node *newNode = new Node( name, m_ids, true, true, m_parent );

    for ( ui32 j = 0; j < node->mNumMeshes; j++ ) {
        const ui32 meshIdx = node->mMeshes[ j ];
        if ( meshIdx >= m_geoArray.size() ) {
            continue;
        }
        Geometry *geo = m_geoArray[ meshIdx ];
        newNode->addGeometry( geo );
    }

    const ui32 numChildren( node->mNumChildren );
    for ( ui32 i = 0; i < numChildren; i++ ) {
        aiNode *currentNode( node->mChildren[ i ] );
        if ( nullptr == currentNode ) {
            continue;
        }
        pushNode( newNode );
        handleNode( currentNode );
        popNode();
    }
}

void AssimpWrapper::handleMaterial( aiMaterial *material ) {
    if ( nullptr == material ) {
        return;
    }
    
    Material *osreMat( new Material );
    i32 texIndex( 0 );
    aiString texPath;	// contains filename of texture
    //AI_TEXTURE_TYPE_MAX
    if ( AI_SUCCESS == material->GetTexture( aiTextureType_DIFFUSE, texIndex, &texPath ) ) {
        osreMat->m_numTextures++;
        osreMat->m_pTextures = new Texture[ 1 ];
        Texture *tex( &osreMat->m_pTextures[ 0 ] );
        String texname( texPath.C_Str() );
        tex->m_loc = IO::Uri( texname );
        String::size_type pos = texname.rfind( "/" );
        if ( pos != String::npos ) {
            texname = texname.substr( pos, texname.size() - pos );
        }
        tex->m_textureName = texname;
    }

    aiColor4D diffuse;
    if ( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_DIFFUSE, &diffuse ) ) {
    }

    aiColor4D specular;
    if ( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_SPECULAR, &specular ) ) {

    }

    aiColor4D ambient;
    if ( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_AMBIENT, &ambient ) ) {

    }

    aiColor4D emission;
    if ( AI_SUCCESS == aiGetMaterialColor( material, AI_MATKEY_COLOR_EMISSIVE, &emission ) ) {

    }

    ai_real shininess, strength;
    unsigned int max;	// changed: to unsigned
    i32 ret;
    ret = aiGetMaterialFloatArray( material, AI_MATKEY_SHININESS, &shininess, &max );
    ret = aiGetMaterialFloatArray( material, AI_MATKEY_SHININESS_STRENGTH, &strength, &max );
}

} // Namespace Assets
} // Namespace OSRE
