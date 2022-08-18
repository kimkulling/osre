/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Animation/AnimatorBase.h>

#include <osre/App/AssetRegistry.h>
#include <osre/App/ResourceCacheService.h>
#include <osre/App/ServiceProvider.h>
#include <osre/App/World.h>
#include <osre/App/Stage.h>
#include <osre/App/TransformController.h>
#include <osre/Common/Environment.h>
#include <osre/Common/TObjPtr.h>
#include <osre/Debugging/osre_debugging.h>
#include <osre/IO/IOService.h>
#include <osre/Platform/AbstractPlatformEventQueue.h>
#include <osre/Platform/AbstractTimer.h>
#include <osre/Platform/AbstractWindow.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Properties/Settings.h>
#include <osre/RenderBackend/Pipeline.h>
#include <osre/RenderBackend/RenderBackendService.h>
#include <osre/RenderBackend/TransformMatrixBlock.h>
#include <osre/Scene/Camera.h>
#include <osre/Scene/MaterialBuilder.h>

#include "src/Engine/App/MouseEventListener.h"
#include "src/Engine/Platform/PlatformPluginFactory.h"

namespace OSRE {
namespace App {

using namespace ::OSRE::Common;
using namespace ::OSRE::Platform;
using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Scene;
using namespace ::OSRE::Animation;
using namespace ::OSRE::IO;

static const c8 *Tag = "AppBase";

AppBase::AppBase(i32 argc, const c8 *argv[], const String &supportedArgs, const String &desc) :
        mAppState(State::Uninited),
        m_argParser(argc, argv, supportedArgs, desc),
        m_environment(nullptr),
        m_settings(new Properties::Settings),
        m_platformInterface(nullptr),
        m_timer(nullptr),
        m_rbService(nullptr),
        m_worlds(),
        mStage(nullptr),
        m_mouseEvListener(nullptr),
        m_keyboardEvListener(nullptr),
        m_ids(nullptr),
        m_shutdownRequested(false),
        mOsreContext(nullptr) {
    m_settings->setString(Properties::Settings::RenderAPI, "opengl");
    m_settings->setBool(Properties::Settings::PollingMode, true);
}

AppBase::~AppBase() {
    delete m_settings;
    m_settings = nullptr;
}

bool AppBase::initWindow(ui32 x, ui32 y, ui32 width, ui32 height, const String &title, bool fullscreen,
        RenderBackendType renderer) {
    osre_assert(nullptr != m_settings);

    m_settings->setInt(Properties::Settings::WinX, x);
    m_settings->setInt(Properties::Settings::WinY, y);
    m_settings->setInt(Properties::Settings::WinWidth, width);
    m_settings->setInt(Properties::Settings::WinHeight, height);
    m_settings->setBool(Properties::Settings::FullScreen, fullscreen);
    m_settings->setString(Properties::Settings::WindowsTitle, title);
    if (renderer == RenderBackendType::OpenGLRenderBackend) {
        m_settings->setString(Properties::Settings::RenderAPI, "opengl");
    } else if (renderer == RenderBackendType::VulkanRenderBackend) {
        m_settings->setString(Properties::Settings::RenderAPI, "vulkan");
    }

    return onCreate();
}

bool AppBase::create(Properties::Settings *config) {
    if (nullptr != config && config != m_settings) {
        delete m_settings;
        m_settings = config;
    }

    return onCreate();
}

bool AppBase::destroy() {
    if (mAppState == State::Created || mAppState == State::Running) {
        return onDestroy();
    }
    return false;
}

void AppBase::update() {
    if (mAppState == State::Created) {
        mAppState = State::Running;
    }
    
    onUpdate();
}

void AppBase::resize(i32 x, i32 y, i32 w, i32 h) {
    if (nullptr == m_platformInterface) {
        return;
    }

    AbstractWindow *rootWindow = m_platformInterface->getRootWindow();
    if (nullptr == rootWindow) {
        return;
    }

    rootWindow->resize(x, y, w, h);
}

void AppBase::requestNextFrame() {
    osre_assert(nullptr != m_rbService);

    if (mStage == nullptr) {
        return;
    }

    mStage->draw(m_rbService);
    m_rbService->update();
}

bool AppBase::handleEvents() {
    if (nullptr == m_platformInterface) {
        osre_debug(Tag, "AppBase::PlatforInterface not in proper state: not nullptr.");
        return false;
    }

    const bool result = m_platformInterface->update();
    if (shutdownRequested()) {
        return false;
    }

    return result;
}

Properties::Settings *AppBase::getSettings() const {
    return m_settings;
}

Scene::Camera *AppBase::setActiveCamera(Scene::Camera *view) {
    if (nullptr == mStage) {
        osre_debug(Tag, "No world to activate state to.");
        return nullptr;
    }

    World *activeWorld = mStage->getActiveWorld();
    if (activeWorld == nullptr) {
        return nullptr;
    }
    
    return activeWorld->setActiveCamera(view);

}

void AppBase::requestShutdown() {
    m_shutdownRequested = true;
}

bool AppBase::shutdownRequested() const {
    return m_shutdownRequested;
}

Platform::AbstractTimer *AppBase::getActiveTimer() const {
    return m_timer;
}

RenderBackendService *AppBase::getRenderBackendService() const {
    return m_rbService;
}

AnimationControllerBase *AppBase::getTransformController(TransformMatrixBlock &tmb) {
    return new TransformController(tmb);
}

Platform::AbstractWindow *AppBase::getRootWindow() const {
    if (nullptr == m_platformInterface) {
        return nullptr;
    }

    return m_platformInterface->getRootWindow();
}

void AppBase::setWindowsTitle(const String &title) {
    if (nullptr == m_platformInterface) {
        return;
    }

    AbstractWindow *rs = m_platformInterface->getRootWindow();
    if (nullptr != rs) {
        rs->setWindowsTitle(title);
    }
}

bool AppBase::onCreate() {
    if (mAppState != State::Uninited) {
        osre_debug(Tag, "AppBase::State not in proper state: Uninited.");
        return false;
    }

    m_ids = new Common::Ids;
    m_environment = new Common::Environment;

    // create the asset registry
    AssetRegistry *registry = AssetRegistry::create();
    if (nullptr == registry) {
        osre_debug(Tag, "Cannot create asset registry.");
    }

    // create the platform interface instance
    m_platformInterface = Platform::PlatformInterface::create(m_settings);
    if (nullptr == m_platformInterface) {
        osre_error(Tag, "Pointer to platform interface is nullptr.");
        return false;
    }

    if (!m_platformInterface->open()) {
        osre_error(Tag, "Error while opening platform interface.");
        return false;
    }

    // register any available platform-specific log streams
    Common::AbstractLogStream *stream = Platform::PlatformPluginFactory::createPlatformLogStream();
    if (nullptr != stream) {
        Logger::getInstance()->registerLogStream(stream);
    }

    // create the render back-end
    m_rbService = new RenderBackendService();
    m_rbService->setSettings(m_settings, false);
    if (!m_rbService->open()) {
        m_rbService->release();
        m_rbService = nullptr;
        return false;
    }

    // Create our world
    mStage = new Stage("stage");
    mStage->createWorld("world");


    const String &api = m_rbService->getSettings()->getString(Properties::Settings::RenderAPI);
    m_environment->addStrVar("api", api.c_str());

    m_platformInterface->getPlatformEventHandler()->setRenderBackendService(m_rbService);

    // enable render-back-end
    RenderBackend::CreateRendererEventData *data = new CreateRendererEventData(m_platformInterface->getRootWindow());
    data->m_pipeline = m_rbService->createDefaultPipeline();
    m_rbService->sendEvent(&RenderBackend::OnCreateRendererEvent, data);

    m_timer = Platform::PlatformInterface::getInstance()->getTimer();

    Scene::MaterialBuilder::create();
    ResourceCacheService *rcSrv = new ResourceCacheService;

    // Setup onMouse event-listener
    AbstractPlatformEventQueue *evHandler = m_platformInterface->getPlatformEventHandler();
    if (nullptr != evHandler) {
        TArray<const Common::Event *> eventArray;
        eventArray.add(&MouseButtonDownEvent);
        eventArray.add(&MouseButtonUpEvent);
        eventArray.add(&MouseMoveEvent);
        m_mouseEvListener = new MouseEventListener;
        evHandler->registerEventListener(eventArray, m_mouseEvListener);

        eventArray.resize(0);
        eventArray.add(&KeyboardButtonDownEvent);
        eventArray.add(&KeyboardButtonUpEvent);
        m_keyboardEvListener = new KeyboardEventListener;
        evHandler->registerEventListener(eventArray, m_keyboardEvListener);
    }

    IO::IOService::create();
#ifdef OSRE_WINDOWS
    App::AssetRegistry::registerAssetPath("assets", "../../assets");
#else
    App::AssetRegistry::registerAssetPath("assets", "../assets");
#endif

    ServiceProvider::create(m_rbService, rcSrv, IOService::getInstance());

    mAppState = State::Created;
    osre_debug(Tag, "Set application state to Created.");

    return true;
}

bool AppBase::onDestroy() {
    if (mAppState != State::Running) {
        osre_debug(Tag, "AppBase::State not in proper state: Running.");
        return false;
    }

    AbstractPlatformEventQueue *evHandler = m_platformInterface->getPlatformEventHandler();
    if (nullptr != evHandler) {
        Common::EventPtrArray eventArray;
        eventArray.add(&MouseButtonDownEvent);
        eventArray.add(&MouseButtonUpEvent);
        eventArray.add(&KeyboardButtonDownEvent);
        eventArray.add(&KeyboardButtonUpEvent);
        evHandler->unregisterAllEventHandler(eventArray);
    }
    AssetRegistry::destroy();
    ServiceProvider::destroy();

    if (m_platformInterface) {
        Platform::PlatformInterface::destroy();
        m_platformInterface = nullptr;
    }

    Scene::MaterialBuilder::destroy();

    delete mStage;
    mStage = nullptr;

    delete m_ids;
    m_ids = nullptr;

    delete m_mouseEvListener;
    m_mouseEvListener = nullptr;

    delete m_keyboardEvListener;
    m_keyboardEvListener = nullptr;

    osre_debug(Tag, "Set application state to destroyed.");
    mAppState = State::Destroyed;
    Logger::kill();

    return true;
}

static constexpr i64 Conversion2Micro = 1000;

void AppBase::onUpdate() {
    i64 microsecs = m_timer->getMilliCurrentSeconds() * Conversion2Micro;
    Time dt(microsecs);    

    if (nullptr != mStage) {
        mStage->update(dt);
    }

    m_keyboardEvListener->clearKeyMap();
}

void AppBase::onRender() {
    if (nullptr != mStage) {
        mStage->draw(m_rbService);
    }
}

const ArgumentParser &AppBase::getArgumentParser() const {
    return m_argParser;
}

Ids *AppBase::getIdContainer() const {
    return m_ids;
}

bool AppBase::isKeyPressed(Key key) const {
    if (nullptr == m_keyboardEvListener) {
        return false;
    }

    return m_keyboardEvListener->isKeyPressed(key);
}

void AppBase::getResolution(ui32 &width, ui32 &height) {
    width = height = 0;
    Rect2ui windowsRect;
    Platform::AbstractWindow *rootWindow = getRootWindow();
    if (nullptr == rootWindow) {
        return;
    }

    rootWindow->getWindowsRect(windowsRect);
    width = windowsRect.getWidth();
    height = windowsRect.getHeight();
}

} // Namespace App
} // Namespace OSRE
