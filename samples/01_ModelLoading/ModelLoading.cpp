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
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/Scene/View.h>
#include <osre/Assets/AssetRegistry.h>
#include <osre/Assets/AssimpWrapper.h>
#include <osre/Assets/Model.h>
#include <osre/Assets/Model.h>
#include <osre/IO/Uri.h>
#include <osre/Platform/AbstractSurface.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/Common/Ids.h>
#include <osre/Scene/GeometryBuilder.h>

#include <glm/gtc/matrix_transform.hpp>

using namespace ::OSRE;
using namespace ::OSRE::Assets;
using namespace ::OSRE::Common;
using namespace ::OSRE::RenderBackend;

// To identify local log entries 
static const String Tag = "ModelLoadingApp"; 

static const String ModelPath = "file://assets/Models/Obj/box.obj";
//static const String ModelPath = "file://assets/Models/Obj/spider.obj";

// The example application, will create the render environment and render a simple triangle onto it
class ModelLoadingApp : public App::AppBase {
    Scene::Stage *m_stage;
    Scene::View  *m_view;
    f32 m_angle;
    TransformMatrixBlock m_transformMatrix;

public:
    ModelLoadingApp( int argc, char *argv[] )
    : AppBase( argc, argv )
    , m_stage( nullptr )
    , m_view( nullptr )
    , m_angle( 0.0f )
    , m_transformMatrix() {
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
            Platform::AbstractSurface *rootSurface( getRootSurface() );
            if ( nullptr == rootSurface ) {
                return false;
            }
            
            const i32 w = rootSurface->getProperties()->m_width;
            const i32 h = rootSurface->getProperties()->m_height;
            f32 aspect = static_cast< f32 >( w ) / static_cast< f32 >( h );

            f32 zNear = 0.0001f;
            f32 zFar  = 100.f;
            m_view->setProjectionMode(glm::radians(45.0f), aspect, zNear, zFar );
            glm::vec3 eye( 2 * diam, 0, 0), up( 1, 0, 0 );
            m_view->setLookAt( eye, glm::vec3( 0,0,0 ), up );

            m_transformMatrix.m_model = glm::mat4(1.0f);
            m_transformMatrix.m_view = m_view->getView();
            m_transformMatrix.m_projection = m_view->getProjection();
            m_transformMatrix.update();
            AppBase::getRenderBackendService()->setMatrix( "MVP", m_transformMatrix.m_mvp );

            AppBase::activateStage( m_stage->getName() );
            Scene::Node *node = m_stage->addNode( "modelNode", nullptr );
            if ( nullptr != node ) {
                node->addModel(model);
            }
        }

        return true;
    }

    /*void onUpdate( d32 timetick ) override {
        glm::mat4 rot( 1.0 );
        //m_transformMatrix.m_model = glm::rotate( rot, m_angle, glm::vec3( 1, 1, 0 ) );
        m_transformMatrix.update();
        m_angle += 0.01f;

        //AppBase::getRenderBackendService()->setMatrix( "MVP", m_transformMatrix.m_mvp );

    }*/

};

OSRE_MAIN( ModelLoadingApp )
