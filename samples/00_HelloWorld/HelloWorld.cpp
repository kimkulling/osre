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
#include <osre/App/AppBase.h>
#include <osre/Properties/Settings.h>
#include <osre/Common/Logger.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Profiling/PerformanceCounterRegistry.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <iomanip>

using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Properties;

// To identify local log entries we will define this tag.
static const String Tag = "HelloWorldApp"; 

/// The example application, will create the render environment and render a simple triangle onto it
class HelloWorldApp : public App::AppBase {
    /// The main state, is used to describe a scene to render
    Scene::Stage *m_stage;
    /// The transform block, contains the model-, view- and projection-matrix
    TransformMatrixBlock m_transformMatrix;

public:
    /// The class constructor with the incoming arguments from the command line.
    HelloWorldApp( int argc, char *argv[] )
    : AppBase( argc, argv )
    , m_stage( nullptr ) {
        // empty
    }

    /// The class destructor.
    virtual ~HelloWorldApp() {
        // empty
    }

protected:
    bool onCreate( Settings *settings = nullptr ) override {
        Settings *baseSettings( AppBase::getSettings() );
        if ( nullptr == baseSettings ) {
            return false;
        }
       
        baseSettings->setString( Properties::Settings::WindowsTitle, "HelloWorld!" );
        baseSettings->setString(Settings::RenderAPI, "dx11");
        if ( !AppBase::onCreate( baseSettings ) ) {
            return false;
        }

#ifdef OSRE_WINDOWS
        Assets::AssetRegistry::registerAssetPath( "assets", "../../media" );
#else
        Assets::AssetRegistry::registerAssetPath( "assets", "../media" );
#endif 

        m_stage = AppBase::createStage( "HelloWorld" );
        AppBase::activateStage( m_stage->getName() );

        Scene::Node *geoNode = m_stage->createNode( "geo", nullptr );
        const Scene::Node::AABB &aabb = geoNode->getAABB();

        Scene::GeometryBuilder myBuilder;
        myBuilder.allocTriangles(VertexType::ColorVertex, BufferAccessType::ReadOnly);
        RenderBackend::Geometry *geo = myBuilder.getGeometry();
        if( nullptr != geo ) {
			m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
            m_transformMatrix.update();
            getRenderBackendService()->setMatrix( "MVP", m_transformMatrix.m_mvp );
            geoNode->addGeometry( geo );
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
