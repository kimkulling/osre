/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Assets/Model.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Scene/Node.h>

namespace OSRE {
namespace Assets {

using namespace ::OSRE::Collision;

Model::Model() 
: m_geoArray()
, m_root( nullptr )
, m_aabb() {
    // empty
}

Model::Model( GeoArray *geoArray, Scene::Node *root, ModelAABB &aabb )
: m_geoArray( *geoArray )
, m_root( root )
, m_aabb( aabb ) {
    // empty
}

Model::~Model() {
    // empty
}

void Model::setGeoArray( GeoArray &geoArray ) {
    m_geoArray = geoArray;
}

const Model::GeoArray &Model::getGeoArray() const {
    return m_geoArray;
}

void Model::setRootNode( Scene::Node *root ) {
    if ( nullptr != m_root ) {
        m_root->release();
    }
    m_root = root;
    if ( nullptr != m_root ) {
        m_root->get();
    }
}

Scene::Node *Model::getRootNode() const {
    return m_root;
}

void Model::setAABB( const TAABB<f32> &aabb ) {
    m_aabb = aabb;
}

const TAABB<f32> &Model::getAABB() const {
    return m_aabb;
}

} // Namespace Assets
} // Namespace OSRE
