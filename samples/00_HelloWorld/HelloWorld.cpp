/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "App/App.h"
#include "RenderBackend/RenderCommon.h"
#include "RenderBackend/MeshBuilder.h"
#include "RenderBackend/Mesh.h"
#include "RenderBackend/MaterialBuilder.h"
#include "RenderBackend/RenderBackendService.h"
#include "RenderBackend/TransformMatrixBlock.h"
#include "Animation/AnimatorComponent.h"

// We want to use the App component and the render backend, so use these namespaces
using namespace ::OSRE;
using namespace ::OSRE::App;
using namespace ::OSRE::RenderBackend;

// To identify local log entries we will define this tag.
static constexpr c8 Tag[] = "HelloWorldApp";

const String VsSrc =
        "#version 400 core\n"
        "\n"
        "layout(location = 0) in vec3 position;	            // object space vertex position\n"
        "layout(location = 1) in vec3 normal;	            // object space vertex normal\n"
        "layout(location = 2) in vec3 color0;	            // object space vertex normal\n"
        "layout(location = 3) in vec2 texcoord0;	        // texture coordinate\n"
        "\n"
        "//output from the vertex shader\n"
        "smooth out vec4 vSmoothColor\n;		            //smooth colour to fragment shader\n"
        "smooth out vec2 vUV;\n"
        "\n"
        "//uniform\n"
        "uniform mat4 MVP;	                                //combined modelview projection matrix\n"
        "\n"
        "void main() {\n"
        "    // assign the per-vertex color to vSmoothColor varying\n"
        "    vSmoothColor = vec4(color0,1);\n"
        "\n"
        "    // get the clip space position by multiplying the combined MVP matrix with the object space\n"
        "    // vertex position\n"
        "    gl_Position = MVP*vec4(position,1);\n"
        "    vUV = texcoord0;\n"
        "}\n";

const String FsSrc =
        "#version 400 core\n"
        "\n"
        "layout(location=0) out vec4 vFragColor; //fragment shader output\n"
        "\n"
        "//input form the vertex shader\n"
        "smooth in vec4 vSmoothColor;		    //interpolated colour to fragment shader\n"
        "smooth in vec2 vUV;\n"
        "uniform sampler2D tex0;\n"
        "\n"
        "void main() {\n"
        "    //set the interpolated color as the shader output\n"
        "    vFragColor = texture( tex0, vUV );\n"
        "}\n";

//-------------------------------------------------------------------------------------------------
///	@ingroup    Samples
///
/// @brief The example application.
///
/// It will create the render environment and render a simple cube onto it.
//-------------------------------------------------------------------------------------------------
class HelloWorldApp : public App::AppBase {
    /// The transform block, contains the model-, view- and projection-matrix
    TransformMatrixBlock mTransformMatrix;
    /// The entity to render
    Entity *mEntity;
    /// The keyboard controller instance.
    Animation::AnimationControllerBase *mKeyboardTransCtrl = nullptr;

public:
    /// The class constructor with the incoming arguments from the command line.
    HelloWorldApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv),
            mEntity(nullptr) {
        // empty
    }

    /// The class destructor.
    ~HelloWorldApp() override = default;

protected:
    CameraComponent *setupCamera(Scene *scene) {
        Entity *camEntity = new Entity("camera", *getIdContainer(), scene);
        scene->addEntity(camEntity);
        CameraComponent *camera =(CameraComponent*) camEntity->createComponent(ComponentType::CameraComponentType);
        scene->setActiveCamera(camera);
        
        ui32 w, h;
        AppBase::getResolution(w, h);
        camera->setProjectionParameters(60.f, (f32)w, (f32)h, 0.001f, 1000.f);

        return camera;
    }

    bool onCreate() override {
        if (!AppBase::onCreate()) {
            return false;
        }

        AppBase::setWindowsTitle("Hello-World sample! Rotate with keyboard: w, a, s, d, scroll with q, e");
        Scene *scene = new Scene("hello_world", *getIdContainer());
        addScene(scene, true);
        mEntity = scene->createEntity("entity");

        MeshBuilder meshBuilder;
        Mesh *mesh = meshBuilder.createCube(VertexType::RenderVertex, .5,.5,.5,BufferAccessType::ReadOnly).getMesh();
        TextureResourceArray texResArray;
        TextureResource *texRes = new TextureResource("SpiderTex", IO::Uri("file://assets/Models/Obj/SpiderTex.jpg"));
        texResArray.add(texRes);
        Material *material = MaterialBuilder::createTexturedMaterial("SpiderTex", texResArray, VsSrc, FsSrc);
        mesh->setMaterial(material);
        if (mesh != nullptr) {
            RenderComponent *rc = (RenderComponent*) mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);

            CameraComponent *camera = setupCamera(scene);
            scene->init();
            camera->observeBoundingBox(mEntity->getAABB());
        }
        mKeyboardTransCtrl = AppBase::getTransformController(mTransformMatrix);

        osre_info(Tag, "Creation finished.");

        return true;
    }

    void onUpdate() override {
        Platform::Key key = AppBase::getKeyboardEventListener()->getLastKey();
        if (key != Platform::KEY_UNKNOWN) {
            mKeyboardTransCtrl->update(mKeyboardTransCtrl->getKeyBinding(key));
        }

        RenderBackendService *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        
        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        {
            rbSrv->beginRenderBatch("b1");
            rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.mModel);

            rbSrv->endRenderBatch();
        }
        rbSrv->endPass();

        AppBase::onUpdate();
    }
};

int main( int argc, char *argv[] )  {
    HelloWorldApp myApp( argc, argv );
    if ( !myApp.create() ) {
        return 1;
    }
    while ( myApp.handleEvents() ) {
        myApp.update();
        myApp.requestNextFrame();
    }
    myApp.destroy();
    return 0;
}
