/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"
#include <stdio.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>

#define main main

#include <osre/App/App.h>
#include <osre/RenderBackend/RenderCommon.h>
#include <osre/RenderBackend/MeshBuilder.h>
#include <osre/Common/Logger.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/TransformMatrixBlock.h>
#include <osre/App/Entity.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Common/glm_common.h>

using namespace OSRE;
using namespace OSRE::RenderBackend;
using namespace ::OSRE::App;

static constexpr c8 Tag[] = "HelloWorldApp";

class OsreEd : public App::AppBase {
    /// The transform matrix block
    TransformMatrixBlock mTransformMatrix;
    /// The entity to render
    Entity *mEntity;
    /// The keyboard controller instance.
    Animation::AnimationControllerBase *mKeyboardTransCtrl;

public:
    OsreEd(int argc, char *argv[]) :
            AppBase(argc, (const char **)argv, "api", "The render API") {}
    
    ~OsreEd() override = default;

    Camera *setupCamera(World *world) {
        Entity *camEntity = new Entity("camera", *getIdContainer(), world);
        world->addEntity(camEntity);
        Camera *camera = (Camera *)camEntity->createComponent(ComponentType::CameraComponentType);
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

        AppBase::setWindowsTitle("Hello-World sample! Rotate with keyboard: w, a, s, d, scroll with q, e");
        World *world = getStage()->addActiveWorld("hello_world");
        mEntity = new Entity("entity", *AppBase::getIdContainer(), world);
        Camera *camera = setupCamera(world);

        MeshBuilder meshBuilder;
        RenderBackend::Mesh *mesh = meshBuilder.createCube(VertexType::ColorVertex, .5, .5, .5, BufferAccessType::ReadOnly).getMesh();
        if (nullptr != mesh) {
            RenderComponent *rc = (RenderComponent *)mEntity->getComponent(ComponentType::RenderComponentType);
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
    std::cout << "Editor version 0.1\n";

    OsreEd osreApp(argc, argv);
    if (!osreApp.initWindow(100, 100, 200, 200, "test", false, App::RenderBackendType::OpenGLRenderBackend)) {
        return -1;
    }

    osreApp.create(nullptr);
    
    // Setup SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Decide GL+GLSL versions
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 130";

#ifdef __APPLE__
    // GL 3.2 Core + GLSL 150
    glsl_version = "#version 150";
    SDL_GL_SetAttribute( // required on Mac OS
            SDL_GL_CONTEXT_FLAGS,
            SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
#elif __linux__
    // GL 3.2 Core + GLSL 150
    glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
#elif _WIN32
    // GL 3.0 + GLSL 130
    glsl_version = "#version 130";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, 0);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
#endif

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window *window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL3 example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static int counter = 0;

            ImGui::Begin("OSRE-Viewer"); // Create a window called "Hello, world!" and append into it.
            ImGui::Text("World"); // Display some text (you can use a format strings too)

            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
        osreApp.handleEvents();
        osreApp.update();
        osreApp.requestNextFrame();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
