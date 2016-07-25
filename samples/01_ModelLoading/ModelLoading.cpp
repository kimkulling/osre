/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace OSRE;
using namespace OSRE::RenderBackend;

// To identify local log entries 
static const String Tag = "ModelLoadingApp"; 

// The example application, will create the render environment and render a simple triangle onto it
class ModelLoadingApp : public App::AppBase {
    Scene::Stage *m_stage;
    TransformMatrixBlock m_transformMatrix;

public:
    ModelLoadingApp( int argc, char *argv[] )
    : AppBase( argc, argv )
    , m_stage( nullptr ) {
        // empty
    }

    virtual ~ModelLoadingApp() {
        // empty
    }

protected:
    void createStaticText() {

    }

    virtual bool onCreate( Properties::Settings *settings = nullptr ) {
        Properties::Settings *baseSettings( AppBase::getSettings() );
        if ( nullptr == baseSettings ) {
            return false;
        }
       
        baseSettings->setString( Properties::Settings::WindowsTitle, "HelloWorld!" );
        if ( !AppBase::onCreate( baseSettings ) ) {
            return false;
        }

#ifdef OSRE_WINDOWS
        Assets::AssetRegistry::getInstance()->registerAssetPath( "assets", "../../media" );
#else
        Assets::AssetRegistry::getInstance()->registerAssetPath( "assets", "../media" );
#endif 

        m_stage = AppBase::createStage( "HelloWorld" );
        AppBase::activateStage( m_stage->getName() );

        Scene::Node *geoNode = m_stage->addNode( "geo", nullptr );
        Scene::GeometryBuilder myBuilder;
        RenderBackend::StaticGeometry *geo = myBuilder.allocTriangles( ColorVertex );
        if( nullptr != geo ) {
			m_transformMatrix.m_model = glm::rotate(m_transformMatrix.m_model, 0.0f, glm::vec3(1, 1, 0));

			Parameter *parameter = Parameter::create( "MVP", PT_Mat4 );
			::memcpy(parameter->m_data.m_data, glm::value_ptr(m_transformMatrix.m_projection*m_transformMatrix.m_view*m_transformMatrix.m_model), sizeof(glm::mat4));

			geo->m_material->m_parameters = parameter;
            geo->m_material->m_numParameters++;
			//geo->m_numParameter++;

			geoNode->addGeometry( geo );
		}

        return true;
    }
};

OSRE_MAIN( ModelLoadingApp )
