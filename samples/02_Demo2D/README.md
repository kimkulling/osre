## The User-Interface-Demo

This example shows how to use the simple engine-intern User-Interface-Framework.

So lets start with the basic application:

```cpp

class Demo2DApp : public App::AppBase {
    /// The transform matrices
    TransformMatrixBlock  mTransformMatrix;
    /// The 2D renderer for the ui overlay
    CanvasRenderer *mCanvasRenderer;
    /// The entity to render the 3d-scene
    Entity *mEntity;

public:
    Demo2DApp(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv),
            mTransformMatrix(),
            mCanvasRenderer(nullptr),
            mEntity(nullptr) {
        // empty
    }

    ~Demo2DApp() override {
        delete mCanvasRenderer;
    }

    void quitCallback(ui32, void *) {
        AppBase::requestShutdown();
    }

protected:
    CameraComponent *setupCamera(Scene *scene) {
        Entity *camEntity = new Entity("camera", *getIdContainer(), scene);
        scene->addEntity(camEntity);
        CameraComponent *camera = (CameraComponent *)camEntity->createComponent(ComponentType::CameraComponentType);
        scene->setActiveCamera(camera);

        ui32 w, h;
        AppBase::getResolution(w, h);
        camera->setProjectionParameters(60.f, (f32)w, (f32)h, 0.001f, 1000.f);

        return camera;
    }

    bool onCreate() override {
        Properties::Settings *baseSettings  = AppBase::getSettings();
        if (baseSettings == nullptr) {
            return false;
        }

        baseSettings->setString(Properties::Settings::WindowsTitle, "Demo in 2D!");
        if (!AppBase::onCreate()) {
            return false;
        }
        
        mCanvasRenderer = AppBase::getCanvasRenderer();
        
        mCanvasRenderer->selectLayer(0);
        const Color4 Red(1, 0, 0, 0);
        mCanvasRenderer->setColor(Red);
        mCanvasRenderer->drawRect(10, 10, 500, 40, true);
        
        Color4 green(0, 1, 0, 1);
        
        mCanvasRenderer->setColor(Red);
        mCanvasRenderer->drawRect(1, 1, 1000, 760, false);

        mCanvasRenderer->setColor(green);
        mCanvasRenderer->drawRect(100, 100, 100, 40, true);

        mCanvasRenderer->drawRect(100, 200, 90, 50, true);

        mCanvasRenderer->drawRect(100, 300, 80, 60, true);

        mCanvasRenderer->drawRect(100, 400, 70, 70, true);

        mCanvasRenderer->drawRect(100, 500, 60, 60, false);

        mCanvasRenderer->selectLayer(2);
        Color4 yellow(1, 1, 0, 1);
        mCanvasRenderer->setColor(yellow);
        mCanvasRenderer->drawRect(100, 100, 110, 30, true);

        mCanvasRenderer->drawRect(100, 600, 110, 124, true);

        mCanvasRenderer->drawRect(100, 700, 110, 124, true);
        
        mCanvasRenderer->drawRect(100, 1000, 110, 124, true);

        mCanvasRenderer->drawText(300, 100, "Test");

        Scene *scene = new Scene("hello_world");
        addScene(scene, true);
        mEntity = new Entity("entity", *AppBase::getIdContainer(), scene);
        MeshBuilder meshBuilder;
        Mesh *mesh = meshBuilder.createCube(VertexType::ColorVertex, .5, .5, .5, BufferAccessType::ReadOnly).getMesh();
        if (mesh != nullptr) {
            RenderComponent *rc = (RenderComponent *)mEntity->getComponent(ComponentType::RenderComponentType);
            rc->addStaticMesh(mesh);

            CameraComponent *camera = setupCamera(scene);
            scene->init();
            camera->observeBoundingBox(mEntity->getAABB());
        }

        return true;
    }

    bool onDestroy() override {
        delete mCanvasRenderer;
        mCanvasRenderer = nullptr;

        return true;
    }

    void onUpdate() {
        RenderBackendService *rbSerive = ServiceProvider::getService<RenderBackendService>(ServiceType::RenderService);
        
        rbSerive->beginPass(RenderPass::getPassNameById(RenderPassId));
        rbSerive->beginRenderBatch("b1");
        rbSerive->setMatrix(MatrixType::Model, mTransformMatrix.mModel);
        rbSerive->endRenderBatch();
        rbSerive->endPass();

        rbSerive->beginPass(RenderPass::getPassNameById(UiPassId));
        rbSerive->beginRenderBatch("2d.b1");
        rbSerive->setMatrix(MatrixType::Model, mTransformMatrix.mModel);
        mCanvasRenderer->render(rbSerive);
        rbSerive->endRenderBatch();
        rbSerive->endPass();

        AppBase::onUpdate();
    }
};

int main(int argc, char *argv[]) {
    Demo2DApp myApp(argc, argv);
    if (!myApp.create()) {
        return 1;
    }

    while (myApp.handleEvents()) {
        myApp.update();
        myApp.requestNextFrame();
    }
    myApp.destroy();

    return 0;
}

```
At first you have to create your application window with your render-window. 
To be able to render any UI-Elements you have to create a screen first. A screen represents a 2D-rect
on you screen.

The next step is to create your dialog: defining a panel, add two buttons and assign callbacks to them
In this case the button on the left will open a FileOpen-dialog and the second button will destroy your 
application.

# Todo
- Optimize UI-elements styling
- Add button texts
- Add button images
- Add onClicked animation
- Implement OpenFile-dialog for other platforms
