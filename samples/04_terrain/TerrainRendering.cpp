#include <osre/App/AppBase.h>
#include <osre/App/CameraComponent.h>
#include <osre/App/Entity.h>
#include <osre/App/World.h>
#include <osre/App/Stage.h>
#include <osre/App/ServiceProvider.h>
#include <osre/App/TransformController.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/MeshBuilder.h>
#include <osre/RenderBackend/TransformMatrixBlock.h>

using namespace ::OSRE;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::App;

// To identify local log entries we will define this tag.
static constexpr c8 Tag[] = "TerrainRenderingApp";

class TerrainRenderingApp : public App::AppBase {
    /// The transform block, contains the model-, view- and projection-matrix
    TransformMatrixBlock mTransformMatrix;
    /// The entity to render
    Entity *mEntity;
    /// The keyboard controller instance.
    Animation::AnimationControllerBase *mKeyboardTransCtrl;

public:
    TerrainRenderingApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv),
            mTransformMatrix(),
            mEntity(nullptr),
            mKeyboardTransCtrl(nullptr) {
        // empty
    }
    
    ~TerrainRenderingApp() override {
        delete mEntity;
    }

protected:
    bool loadHeightMap(const String &filename) {
        if (filename.empty()) {
            return false;
        }

        
    }
    Camera *setupCamera(World *world) {
        Entity *camEntity = new Entity("camera", *getIdContainer(), world);
        world->addEntity(camEntity);
        Camera *camera =(Camera*) camEntity->createComponent(ComponentType::CameraComponentType);
        world->setActiveCamera(camera);
        ui32 w, h;
        AppBase::getResolution(w, h);
        camera->setProjectionParameters(60.f, (f32)w, (f32)h, 0.001f, 1000.f);
        
        return camera;
    }

    bool onCreate() override {
        if (!AppBase::onCreate()) {
            return false;
        }

        AppBase::setWindowsTitle("Terrain sample! Rotate with keyboard: w, a, s, d, scroll with q, e");
        World *world = getStage()->addActiveWorld("hello_world");
        mEntity = new Entity("entity", *AppBase::getIdContainer(), world);
        Camera *camera = setupCamera(world);

        
        MeshBuilder meshBuilder;
        RenderBackend::Mesh *mesh = meshBuilder.createCube(VertexType::ColorVertex, .5,.5,.5,BufferAccessType::ReadOnly).getMesh();
        if (nullptr != mesh) {
            RenderComponent *rc = (RenderComponent*) mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);

            Time dt;
            world->update(dt);
            camera->observeBoundingBox(mEntity->getAABB());
        }
        mKeyboardTransCtrl = AppBase::getTransformController(mTransformMatrix);

        osre_info(Tag, "Creation finished.");

        return true;
    }

    void onUpdate() override {
        Platform::Key key = AppBase::getKeyboardEventListener()->getLastKey();
        mKeyboardTransCtrl->update(TransformController::getKeyBinding(key));

        RenderBackendService *rbSrv = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        rbSrv->beginPass(RenderPass::getPassNameById(RenderPassId));
        rbSrv->beginRenderBatch("b1");

        rbSrv->setMatrix(MatrixType::Model, mTransformMatrix.m_model);

        rbSrv->endRenderBatch();
        rbSrv->endPass();

        AppBase::onUpdate();
    }
};

int main(int argc, char *argv[]) {
    TerrainRenderingApp myApp(argc, argv);
    if (!myApp.initWindow(10, 10, 1024, 768, "Instancing-Sample", false, false, RenderBackendType::OpenGLRenderBackend)) {
        return 1;
    }

    while (myApp.handleEvents()) {
        myApp.update();
        myApp.requestNextFrame();
    }

    myApp.destroy();

    return 0;
}
