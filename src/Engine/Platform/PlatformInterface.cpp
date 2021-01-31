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
#include <osre/Common/EventTriggerer.h>
#include <osre/Platform/PlatformInterface.h>
#include <osre/Properties/Settings.h>
#include <osre/UI/UiItemFactory.h>
#include <src/Engine/Platform/PlatformPluginFactory.h>
#ifdef OSRE_WINDOWS
#include <src/Engine/Platform/win32/Win32OGLRenderContext.h>
#endif // OSRE_WINDOWS
#include "Engine/Platform/sdl2/SDL2EventQueue.h"
#include "Engine/Platform/sdl2/SDL2Window.h"
#include <src/Engine/Platform/sdl2/SDL2OGLRenderContext.h>
#include <src/Engine/Platform/sdl2/SDL2SystemInfo.h>
#include <src/Engine/Platform/sdl2/SDL2Timer.h>

#include <GL/glew.h>
#include <SDL.h>

namespace OSRE {
namespace Platform {

using namespace ::OSRE::Common;
using namespace ::OSRE::Properties;
using namespace ::OSRE::UI;

PlatformInterface *PlatformInterface::sInstance(nullptr);

ApplicationContext::ApplicationContext(const Settings *settings) :
        mSettings(settings)
#ifdef OSRE_WINDOWS
        ,
        m_type(PluginType::WindowsPlugin)
#else
        ,
        m_type(PluginType::SDL2Plugin)
#endif // OSRE_WINDOWS
        ,
        m_rootSurface(nullptr),
        m_oseventHandler(nullptr),
        m_renderContext(nullptr),
        m_timer(nullptr),
        m_dynLoader(nullptr),
        m_systemInfo(nullptr) {
    // empty
}

ApplicationContext::~ApplicationContext() {
    mSettings = nullptr;
}

static const String PlatformPluginName[static_cast<i32>(PluginType::MaxPlugin)] = {
#ifdef OSRE_WINDOWS
    "WindowsPlugin"
#else
    "SDL2Plugin"
#endif // OSRE_WINDOWS
};

static const c8 *Tag = "PlatformInterface";

PlatformInterface::PlatformInterface(const Settings *config) :
        AbstractService("platform/platforminterface"), mContext(nullptr) {
    mContext = new ApplicationContext(config);
}

PlatformInterface::~PlatformInterface() {
    delete mContext;
    mContext = nullptr;
}

PlatformInterface *PlatformInterface::create(const Settings *config) {
    if (nullptr == sInstance) {
        sInstance = new PlatformInterface(config);
    }

    return sInstance;
}

void PlatformInterface::destroy() {
    if (nullptr == sInstance) {
        return;
    }

    sInstance->close();
    delete sInstance;
    sInstance = nullptr;
}

PlatformInterface *PlatformInterface::getInstance() {
    return sInstance;
}

AbstractPlatformEventQueue *PlatformInterface::getPlatformEventHandler() const {
    return mContext->m_oseventHandler;
}

AbstractOGLRenderContext *PlatformInterface::getRenderContext() const {
    return mContext->m_renderContext;
}

AbstractWindow *PlatformInterface::getRootWindow() const {
    return mContext->m_rootSurface;
}

AbstractTimer *PlatformInterface::getTimer() const {
    return mContext->m_timer;
}

AbstractDynamicLoader *PlatformInterface::getDynamicLoader() const {
    return mContext->m_dynLoader;
}

AbstractSystemInfo *PlatformInterface::getSystemInfo() const {
    return mContext->m_systemInfo;
}

const String &PlatformInterface::getDefaultFontName() const {
    if (nullptr == mContext->mSettings) {
        static const String dummy("none");
        return dummy;
    }

    static const String font(mContext->mSettings->get(Settings::DefaultFont).getString());
    return font;
}

PluginType PlatformInterface::getOSPluginType() {
#ifdef OSRE_WINDOWS
    return PluginType::WindowsPlugin;
#else
    return PluginType::SDL2Plugin;
#endif // OSRE_WINDOWS
}

String PlatformInterface::getOSPluginName(PluginType type) {
    String name("None");
    switch (type) {
#ifdef OSRE_WINDOWS
        case PluginType::WindowsPlugin:
            name = PlatformPluginName[static_cast<i32>(PluginType::WindowsPlugin)];
            break;
#else
        case PluginType::SDL2Plugin:
            name = PlatformPluginName[static_cast<i32>(PluginType::SDL2Plugin)];
            break;
#endif // OSRE_WINDOWS
        default:
            break;
    }

    return name;
}

bool PlatformInterface::onOpen() {
    if (!mContext->mSettings) {
        assert(nullptr != mContext->mSettings);
        osre_debug(Tag, "Invalid pointer to configuration.");
        return false;
    }

    Settings::WorkingModeType appType =
            static_cast<Settings::WorkingModeType>(mContext->mSettings->get(Settings::AppType).getInt());

    WindowsProperties *props(nullptr);
    bool polls(false);
    const Properties::Settings *config = mContext->mSettings;
    if (appType == Settings::GfxApp) {
        // get the configuration values for the window
        props = new WindowsProperties;
        bool fullscreen = false;
        props->m_x = config->get(Settings::WinX).getInt();
        props->m_y = config->get(Settings::WinY).getInt();
        props->m_width = config->get(Settings::WinWidth).getInt();
        props->m_height = config->get(Settings::WinHeight).getInt();
        props->m_colordepth = config->get(Settings::BPP).getByte();
        props->m_depthbufferdepth = config->get(Settings::DepthBufferDepth).getByte();
        props->m_stencildepth = config->get(Settings::StencilBufferDepth).getByte();
        props->m_fullscreen = fullscreen;
        props->m_resizable = config->get(Settings::WindowsResizable).getBool();
        props->m_childWindow = config->get(Settings::ChildWindow).getBool();
        props->m_title = config->get(Settings::WindowsTitle).getString();
        polls = config->get(Settings::PollingMode).getBool();
    }

    String appName = "My OSRE-Application";

    PlatformPluginFactory::init();
#ifdef OSRE_WINDOWS
    osre_info(Tag, "Platform plugin created for Windows.");
#else
    osre_info(Tag, "Platform plugin created for Linux.");
#endif
    mContext->m_dynLoader = PlatformPluginFactory::createDynmicLoader();
    bool result(true);
    if (appType == Settings::GfxApp) {
        result = setupGfx(props, polls);
    }

    mContext->m_systemInfo = PlatformPluginFactory::createSystemInfo();

    return result;
}

bool PlatformInterface::onClose() {
    PlatformPluginFactory::release();

    delete mContext->m_oseventHandler;
    mContext->m_oseventHandler = nullptr;

    if (nullptr != mContext->m_renderContext) {
        mContext->m_renderContext->destroy();
        delete mContext->m_renderContext;
        mContext->m_renderContext = nullptr;
    }

    return true;
}

bool PlatformInterface::onUpdate() {
    if (nullptr == mContext->m_oseventHandler) {
        return false;
    }

    return mContext->m_oseventHandler->update();
}

bool PlatformInterface::setupGfx(WindowsProperties *props, bool polls) {
    // create the root surface
    mContext->m_rootSurface = PlatformPluginFactory::createSurface(props);
    if (!mContext->m_rootSurface->create()) {
        delete mContext->m_rootSurface;
        osre_error(Tag, "Error while creating platform root surface.");

        mContext->m_rootSurface = nullptr;
        return false;
    }

    // install the platform event handler
    mContext->m_oseventHandler = PlatformPluginFactory::createPlatformEventHandler(mContext->m_rootSurface);
    if (!mContext->m_oseventHandler) {
        osre_error(Tag, "Error while creating platform event handler.");
        mContext->m_rootSurface->destroy();
        mContext->m_rootSurface = nullptr;
        return false;
    }
    mContext->m_oseventHandler->enablePolling(polls);
    mContext->m_timer = PlatformPluginFactory::createTimer();

    // setup the render context
    mContext->m_renderContext = PlatformPluginFactory::createRenderContext();

    UiItemFactory::createInstance(getRootWindow());

    return true;
}

} // Namespace Platform
} // Namespace OSRE
