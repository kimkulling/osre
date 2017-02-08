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
#include <osre/Scene/MaterialBuilder.h>
#include <osre/Scene/Component.h>
#include <osre/Scene/Node.h>
#include <osre/Collision/TAABB.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace OSRE {
namespace Assets {
    
using namespace ::Assimp;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;
using namespace ::OSRE::Collision;

static const String Tag = "AssimpWrapper";

AssimpWrapper::AssimpWrapper( Common::Ids &ids )
: m_geoArray()
, m_matArray()
, m_model( nullptr )
, m_parent( nullptr )
, m_ids( ids )
, m_mvpParam( nullptr ) {
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

    m_mvpParam = RenderBackend::Parameter::create( "MVP", ParameterType::PT_Mat4 );

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
        handleNode( scene->mRootNode, nullptr );
    }

    return m_model;
}

void AssimpWrapper::handleMesh( aiMesh *mesh ) {
    if ( nullptr == mesh ) {
        return;
    }

    TAABB<f32> aabb = m_model->getAABB();
    Geometry *geo( Geometry::create( 1 ) );
    ui32 numVertices( mesh->mNumVertices );
    RenderVert *vertices = new RenderVert[ numVertices ];
    for ( ui32 i = 0; i < numVertices; i++ ) {
        if ( mesh->HasPositions() ) {
            aiVector3D &vec3 = mesh->mVertices[ i ];
            vertices[ i ].position.x = vec3.x;
            vertices[ i ].position.y = vec3.y;
            vertices[ i ].position.z = vec3.z;

            AssimpWrapper::updateAxisAlignedBBox( vec3, aabb );
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

    const ui32 matIdx( mesh->mMaterialIndex );
    Material *osreMat = m_matArray[ matIdx ];
    geo->m_material = osreMat;
    const ui32 vbSize( sizeof( RenderVert ) * numVertices );
    geo->m_vb = BufferData::alloc( BufferType::VertexBuffer, vbSize, BufferAccessType::ReadOnly );
    geo->m_vb->copyFrom( &vertices[ 0 ], vbSize );

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

    geo->m_numPrimGroups = 1;
    geo->m_pPrimGroups = new PrimitiveGroup[ geo->m_numPrimGroups ];
    geo->m_pPrimGroups[ 0 ].m_indexType = IndexType::UnsignedShort;
    geo->m_pPrimGroups[ 0 ].m_numPrimitives = mesh->mNumFaces * geo->m_numPrimGroups;
    geo->m_pPrimGroups[ 0 ].m_primitive = PrimitiveType::TriangleList;
    geo->m_pPrimGroups[ 0 ].m_startIndex = 0;

    m_geoArray.add( geo );
    m_model->setAABB( aabb );
}

void AssimpWrapper::handleNode( aiNode *node, Node *parent ) {
    if ( nullptr == node ) {
        return;
    }

    if ( nullptr == parent ) {
        m_parent = m_model->getRootNode();
    } else {
        m_parent = parent;
    }
    const String name( node->mName.C_Str() );
    Node *newNode = new Node( name, m_ids, true, true, m_parent );
    m_parent->addChild( newNode );
    RenderComponent *comp = (RenderComponent*) newNode->getComponent( Scene::Node::ComponentType::RenderComponentType );
    CPPCore::TArray<Parameter*> paramArray;
    paramArray.add( m_mvpParam );
    comp->beginPass( paramArray );
    for ( ui32 j = 0; j < node->mNumMeshes; j++ ) {
        const ui32 meshIdx = node->mMeshes[ j ];
        if ( meshIdx >= m_geoArray.size() ) {
            continue;
        }
        Geometry *geo = m_geoArray[ meshIdx ];
        newNode->addGeometry( geo );
    }
    comp->endPass();
    const ui32 numChildren( node->mNumChildren );
    for ( ui32 i = 0; i < numChildren; i++ ) {
        aiNode *currentNode( node->mChildren[ i ] );
        if ( nullptr == currentNode ) {
            continue;
        }
        handleNode( currentNode, newNode );
    }
}

static void setColor4( const aiColor4D &aiCol, Color4 &col ) {
    col.m_r = aiCol.r;
    col.m_g = aiCol.g;
    col.m_b = aiCol.b;
    col.m_a = aiCol.a;
}

static void setTexture( ui32 texIndex, const aiString &texPath, CPPCore::TArray<Texture*> &textures ) {
    Texture *tex = new Texture;
    textures.add( tex );
    String texname( texPath.C_Str() );
    tex->m_loc = IO::Uri( texname );
    String::size_type pos = texname.rfind( "/" );
    if ( pos != String::npos ) {
        texname = texname.substr( pos, texname.size() - pos );
    }
    tex->m_textureName = texname;
}

static void assignTexturesToMat( Material *osreMat, CPPCore::TArray<Texture*> &textures ) {
    if ( nullptr == osreMat ) {
        return;
    }

    osreMat->m_numTextures = textures.size();
    if ( textures.isEmpty() ) {
        return;
    }

    osreMat->m_textures = new Texture *[ osreMat->m_numTextures ];
    for ( ui32 i = 0; i < osreMat->m_numTextures; i++ ) {
        osreMat->m_textures[ i ] = textures[ i ];
    }
}

void AssimpWrapper::handleMaterial( aiMaterial *material ) {
    if ( nullptr == material ) {
        return;
    }
    
    Material *osreMat( MaterialBuilder::createBuildinMaterial( RenderBackend::VertexType::RenderVertex ) );
    m_matArray.add( osreMat );

    i32 texIndex( 0 );
    aiString texPath;	// contains filename of texture
    //AI_TEXTURE_TYPE_MAX
    CPPCore::TArray<Texture*> textures;
    if ( AI_SUCCESS == material->GetTexture( aiTextureType_DIFFUSE, texIndex, &texPath ) ) {
        setTexture(  texIndex, texPath, textures );
    }
    assignTexturesToMat( osreMat, textures );
    /*aiTextureType_SPECULAR = 0x2,
    aiTextureType_AMBIENT = 0x3,
    aiTextureType_EMISSIVE = 0x4,
    aiTextureType_HEIGHT = 0x5,
    aiTextureType_NORMALS = 0x6,
    aiTextureType_SHININESS = 0x7,
    aiTextureType_OPACITY = 0x8,
    aiTextureType_DISPLACEMENT = 0x9,
    aiTextureType_LIGHTMAP = 0xA,
    aiTextureType_REFLECTION = 0xB,*/


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

void AssimpWrapper::updateAxisAlignedBBox( const aiVector3D &pos, Collision::TAABB<f32> &aabb ) {
    Vec3f v( pos.x, pos.y, pos.z );
    aabb.merge( v );
}

} // Namespace Assets
} // Namespace OSRE
