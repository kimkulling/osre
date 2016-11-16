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
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/GeometryBuilder.h>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>

namespace OSRE {
namespace Assets {
    
using namespace Assimp;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;

AssimpWrapper::AssimpWrapper() 
: m_model( nullptr ) {
    // empty
}

AssimpWrapper::~AssimpWrapper() {
    // empty
}

bool AssimpWrapper::importAsset( const IO::Uri &file, ui32 flags ) {
    if ( !file.isValid() ) {
        return false;
    }

    Importer myImporter;
    const aiScene *scene = myImporter.ReadFile( file.getAbsPath() + file.getResource(), flags );
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
    if ( scene->HasMeshes() ) {
        for ( ui32 i = 0; i < scene->mNumMeshes; i++ ) {
            aiMesh *currentMesh( scene->mMeshes[ i ] );
            if ( nullptr == currentMesh ) {
                continue;
            }

            handleMesh( currentMesh );
        }
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
//            vertices[ i ].color0.a = diffuse.a;
        }

        if ( mesh->HasTextureCoords( 0 ) ) {
            aiVector3D &tex0 = mesh->mTextureCoords[ 0 ][ i ];
            vertices[ i ].tex0.x = tex0.x;
            vertices[ i ].tex0.y = tex0.y;
        }
    }

    geo->m_vb = BufferData::alloc( BufferType::VertexBuffer, sizeof( RenderVert ) * numVertices, BufferAccessType::ReadOnly );

    // ToDo! alloc index buffer
    m_model->addGeometry( geo );
}

} // Namespace Assets
} // Namespace OSRE
