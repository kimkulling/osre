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
#include <osre/App/AppBase.h>
#include <osre/App/Entity.h>
#include <osre/App/Component.h>
#include <osre/Properties/Settings.h>
#include <osre/Common/Logger.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/App/AssetRegistry.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Profiling/PerformanceCounterRegistry.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <iomanip>

using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Properties;

// To identify local log entries we will define this tag.
static const c8 *Tag = "HelloWorldApp"; 

/// The example application, will create the render environment and render a simple triangle onto it
class HelloWorldApp : public App::AppBase {
    /// The main state, is used to describe a scene to render
    Scene::Stage *m_stage;
    /// The transform block, contains the model-, view- and projection-matrix
    TransformMatrixBlock m_transformMatrix;

    Entity *m_entity;

public:
    /// The class constructor with the incoming arguments from the command line.
    HelloWorldApp( int argc, char *argv[] )
    : AppBase( argc, (const char ** ) argv )
    , m_stage( nullptr )
    , m_entity( nullptr ){
        // empty
    }

    /// The class destructor.
    virtual ~HelloWorldApp() {
        // empty
    }

protected:
    bool onCreate() override {
        if (!AppBase::onCreate()) {
            return false;
        }

        AppBase::setWindowsTitle( "Hello-World sample!" );

#ifdef OSRE_WINDOWS
        App::AssetRegistry::registerAssetPath( "assets", "../../media" );
#else
        App::AssetRegistry::registerAssetPath( "assets", "../media" );
#endif 

        m_stage = AppBase::createStage( "HelloWorld" );
        AppBase::activateStage( m_stage->getName() );

        m_entity = new Entity("entity", *AppBase::getIdContainer() );
        RenderComponent *rc = (RenderComponent*) m_entity->getComponent( ComponentType::RenderComponentType );
        Scene::Node *geoNode = m_stage->addNode( "geo", nullptr );
        Scene::MeshBuilder meshBuilder;
        meshBuilder.allocTriangles(VertexType::ColorVertex, BufferAccessType::ReadOnly);
        RenderBackend::Mesh *mesh = meshBuilder.getMesh();
        if( nullptr != mesh ) {
			m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
            m_transformMatrix.update();
            getRenderBackendService()->setMatrix( "MVP", m_transformMatrix.m_mvp );
            rc->addStaticMesh(mesh);
            geoNode->addMeshReference( 0 );
		}

        return true;
    }

    void onUpdate() override {
        ui32 fps( 0 );
        Profiling::PerformanceCounterRegistry::queryCounter( "fps", fps );
        std::stringstream stream;
        stream << std::setfill( '0' ) << std::setw( 2 ) << fps;
        //Scene::DbgRenderer::getInstance()->renderDbgText( 10, 10, 1, stream.str() );

        AppBase::onUpdate();
    }
};

/// Will generate the main function.
OSRE_MAIN( HelloWorldApp )
