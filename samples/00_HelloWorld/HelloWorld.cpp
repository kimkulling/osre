#include <osre/App/AppBase.h>
#include <osre/Properties/ConfigurationMap.h>
#include <osre/Common/Logger.h>
#include <osre/Scene/GeometryBuilder.h>
#include <osre/Scene/Stage.h>
#include <osre/Scene/Node.h>
#include <osre/RenderBackend/RenderCommon.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace OSRE;
using namespace OSRE::RenderBackend;

// to identify local log entries 
static const String Tag    = "HelloWorld"; 

glm::mat4 P = glm::mat4( 1 );
glm::mat4 M = glm::mat4( 1 );
glm::mat4 V = glm::mat4( 1 );

class HelloWorldApp : public App::AppBase {
    Scene::Stage *m_stage;
    TransformMatrixBlock m_transformMatrix;

public:
    HelloWorldApp( int argc, char *argv[] )
    : AppBase( argc, argv )
    , m_stage( nullptr ) {

    }

    virtual ~HelloWorldApp() {

    }

protected:
    virtual bool onCreate( Properties::ConfigurationMap *config = nullptr ) {
        AppBase::getConfig()->setString( Properties::ConfigurationMap::WindowsTitle, "HelloWorld!" );

        if( !AppBase::onCreate( config ) ) {
            return false;
        }

        m_stage = AppBase::createStage( "HelloWorld" );
        Scene::Node *geoNode = m_stage->addNode( "geo", nullptr );
        Scene::GeometryBuilder myBuilder;
        RenderBackend::Geometry *geo = myBuilder.createTriangle();
        if( nullptr != geo ) {
            if( nullptr != geo->m_material->m_pShader ) {
                geo->m_material->m_pShader->m_attributes.add( "vVertex" );
                geo->m_material->m_pShader->m_attributes.add( "vDiffuseColor" );
                geo->m_material->m_pShader->m_parameters.add( "MVP" );
            }
        }

        m_transformMatrix.m_model = glm::rotate( m_transformMatrix.m_model, 0.0f, glm::vec3( 1, 1, 0 ) );

        Parameter *parameter = Parameter::create( "MVP", PT_Mat4 );
        ::memcpy( parameter->m_data.m_data, glm::value_ptr( m_transformMatrix.m_projection*m_transformMatrix.m_view*m_transformMatrix.m_model ), sizeof( glm::mat4 ) );

        geo->m_parameter = parameter;
        geo->m_numParameter++;

        geoNode->addGeometry( geo );

        return true;
    }

    virtual void onUpdate()  {
        m_stage->update();
    }
};

int main( int argc, char *argv[] )  {
    HelloWorldApp myApp( argc, argv );
    if( !myApp.create() ) {
        osre_error(Tag, "Cannot create application instance." );
        return 1;
    }
    
    // handle events until application will be terminated
    while( myApp.handleEvents() ) {
        
        // update application data
        myApp.update();

        // request next frame rendering
        myApp.requestNextFrame();
    }
    myApp.destroy();

    return 0;
}
