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
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/View.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/Assets/AssimpWrapper.h>
#include <osre/Assets/Model.h>
#include <osre/IO/Uri.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/Common/Ids.h>

using namespace ::OSRE;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

// To identify local log entries 
static const String Tag = "ModelLoadingApp"; 

static const String ModelPath = "file://assets/Models/Obj/spider.obj";

// The example application, will create the render environment and render a simple triangle onto it
class ModelLoadingApp : public App::AppBase {
    Scene::Stage *m_stage;
    Scene::View  *m_view;
    TransformMatrixBlock m_transformMatrix;

public:
    ModelLoadingApp( int argc, char *argv[] )
    : AppBase( argc, argv )
    , m_stage( nullptr )
    , m_view( nullptr ) {
        // empty
    }

    virtual ~ModelLoadingApp() {
        // empty
    }

protected:
    bool onCreate( Properties::Settings *settings = nullptr ) override {
        Properties::Settings *baseSettings( AppBase::getSettings() );
        if ( nullptr == baseSettings ) {
            return false;
        }
       
        baseSettings->setString( Properties::Settings::WindowsTitle, "Model Loader!" );
        if ( !AppBase::onCreate( baseSettings ) ) {
            return false;
        }

#ifdef OSRE_WINDOWS
        Assets::AssetRegistry::registerAssetPath( "assets", "../../media" );
#else
        Assets::AssetRegistry::registerAssetPath( "assets", "../media" );
#endif 

        Ids ids;
        Assets::AssimpWrapper assimpWrapper( ids );
        IO::Uri modelLoc( ModelPath );
        if ( assimpWrapper.importAsset( modelLoc, 0 ) ) {
            Assets::Model *model = assimpWrapper.getModel();
            Collision::TAABB<f32> aabb = model->getAABB();

            m_stage = AppBase::createStage( "ModelLoader" );
            m_view = m_stage->addView("camera", nullptr );
            const f32 diam = aabb.getDiameter();
            const Vec3f center = aabb.getCenter();
            f32 zNear = 1.0f;
            f32 zFar = zNear + diam;
            f32 left = center.getX() - diam;
            f32 right = center.getX() + diam;
            f32 bottom = center.getY() - diam;
            f32 top = center.getY() + diam;
            m_view->setOrthoMode( left, right, bottom, top, zNear, zFar );
            glm::vec3 eye( 0, 0, 2 * diam ), up( 0, 1, 0 );
            m_view->setLookAt( eye, glm::vec3( center.getX(), center.getY(), center.getZ() ), up );
            m_transformMatrix.m_view = m_view->getView();
            m_transformMatrix.m_projection = m_view->getProjection();
            AppBase::activateStage( m_stage->getName() );
            Scene::Node *node = m_stage->addNode( "modelNode", nullptr );
            m_stage->addModel( model, node );

            Parameter *parameter = Parameter::create( "MVP", ParameterType::PT_Mat4 );
            ::memcpy( parameter->m_data.m_data, glm::value_ptr( m_transformMatrix.m_projection*m_transformMatrix.m_view*m_transformMatrix.m_model ), sizeof( glm::mat4 ) );
            ParameterRegistry::registerParameter( parameter );
            ParameterRegistry::updateParameter( parameter );
            ParameterRegistry::commitChanges();
        }

        return true;
    }
};

OSRE_MAIN( ModelLoadingApp )
