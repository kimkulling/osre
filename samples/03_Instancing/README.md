## Mesh Loader
![A simple box](../../assets/Images/01_box.png)
This sample shows how to import a model using Assimp.

```cpp
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
using namespace ::OSRE::Scene;

// To identify local log entries 
static const c8 *Tag = "ModelLoadingApp"; 

// The file to load
static const String ModelPath = "file://assets/Models/Obj/spider.obj";

// The example application, will create the render environment and render a simple triangle onto it
class ModelLoadingApp : public App::AppBase {
    String               m_assetFolder;
    Scene::Stage        *m_stage;
    Scene::View         *m_view;
    f32                  m_angle;
    Common::Ids          m_ids;
    glm::mat4            m_model;
    TransformMatrixBlock m_transformMatrix;
    Node::NodePtr        m_modelNode;

public:
    ModelLoadingApp( int argc, char *argv[] )
    : AppBase( argc, argv )
    , m_assetFolder("")
    , m_stage( nullptr )
    , m_view( nullptr )
    , m_angle( 0.0f )
    , m_ids()
    , m_transformMatrix()
    , m_modelNode() {
        // empty
    }

    virtual ~ModelLoadingApp() {
        // empty
    }

protected:
        Properties::Settings *appSettings( settings );
        if ( nullptr == appSettings ) {
            appSettings = AppBase::getSettings();
            if ( nullptr == appSettings ) {
                return false;
            }
        }
       
        appSettings->setString( Properties::Settings::WindowsTitle, "Model Loader!" );
        if ( !AppBase::onCreate( appSettings ) ) {
            return false;
        }

        const Common::ArgumentParser &parser = AppBase::getArgumentParser();
        if ( parser.hasArgument( AssetFolderArg ) ) {
            m_assetFolder = parser.getArgument( AssetFolderArg );
        }

#ifdef OSRE_WINDOWS
        Assets::AssetRegistry::registerAssetPath( "assets", "../../assets" );
#else
        Assets::AssetRegistry::registerAssetPath( "assets", "../assets" );
#endif 
        Scene::DbgRenderer::getInstance()->renderDbgText( 0, 0, 2U, "XXX" );

        Ids ids;
        Assets::AssimpWrapper assimpWrapper( ids );
        IO::Uri modelLoc( ModelPath );
        if ( assimpWrapper.importAsset( modelLoc, 0 ) ) {
            Assets::Model *model = assimpWrapper.getModel();
            Collision::TAABB<f32> aabb = model->getAABB();
            const f32 diam = aabb.getDiameter();
            const Vec3f center = aabb.getCenter();

            CPPCore::TArray<Geometry*> geoArray = model->getGeoArray();
            m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );
            m_transformMatrix.update();
            RenderBackendService *rbSrv( getRenderBackendService() );
            if ( nullptr != rbSrv ) {
                Platform::AbstractWindow *rootWindow(getRootWindow());
                if ( nullptr == rootWindow ) {
                    return false;
                }
                const i32 w = rootWindow->getProperties()->m_width;
                const i32 h = rootWindow->getProperties()->m_height;
                const f32 aspect = static_cast<f32>(w) / static_cast<f32>(h);
                const f32 zNear = 0.0001f;
                const f32 zFar = 1000.f;
                
                glm::vec3 eye( diam, 0, 0), up(0, 0, 1);
                glm::vec3 c(center.getX(), center.getY(), center.getZ());
                
                m_transformMatrix.m_model = glm::mat4(1.0f);
                m_transformMatrix.m_projection = glm::perspective(glm::radians(60.0f), aspect, zNear, zFar);
                m_transformMatrix.m_view = glm::lookAt(eye, c, up);
                rbSrv->setMatrix(MatrixType::View, m_transformMatrix.m_view);
                rbSrv->setMatrix(MatrixType::Projection, m_transformMatrix.m_projection);

                renderNodes(model, rbSrv);
            }
            m_stage = AppBase::createStage( "ModelLoading" );
            const String name(model->getRootNode()->getName());
            m_modelNode = m_stage->addNode(name, nullptr, "default" );
        }

        return true;
    }

    void renderNode(Node *currentNode, RenderBackendService *rbSrv) {
        const ui32 numGeo = currentNode->getNumGeometries();
        for (ui32 i = 0; i < numGeo; ++i) {
            rbSrv->attachGeo(currentNode->getGeometryAt(i), 0 );
        }

        for (ui32 i = 0; i < currentNode->getNumChildren(); ++i) {
            Node *current = currentNode->getChildAt(i);
            renderNode(current, rbSrv);
        }
    }

    void renderNodes(Assets::Model *model, RenderBackendService *rbSrv) {
        if (nullptr == model) {
            return;
        }

        Node *root = model->getRootNode();
        if (nullptr == root) {
            return;
        }

        renderNode(root, rbSrv);
    }

    void onUpdate() override {
        // Rotate the model
        glm::mat4 rot( 1.0 );
        m_transformMatrix.m_model = glm::rotate( rot, m_angle, glm::vec3( 1, 1, 0 ) );

        m_angle += 0.01f;
        RenderBackendService *rbSrv( getRenderBackendService() );

        rbSrv->setMatrix( MatrixType::Model, m_transformMatrix.m_model);
        
        AppBase::onUpdate();
    }
};

OSRE_MAIN( ModelLoadingApp )
```
At first we are generating our render window as usual. In the onCreate-Method we will generate the 
assimp-wrapper and import a model with it.

In the onUpdate-callback the model will be rotated.
