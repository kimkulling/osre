/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Properties/Settings.h"
#include "Platform/AbstractWindow.h"
#include "Platform/PlatformInterface.h"
#include "Properties/BasePropertyMap.h"
#include "Properties/Property.h"

#include <cppcore/Common/Variant.h>

namespace OSRE {
namespace Properties {

using namespace ::OSRE::Platform;

static const cppcore::Variant EmptyVariant;
static const c8 *ConfigKeyStringTable[Settings::MaxKonfigKey] = {
    "AppName",
    "AppType",
    "AppVersionMajor",
    "AppVersionMinor",
    "AppVersionPatch",
    "WindowsTitle",
    "RenderAPI",
    "WinX",
    "WinY",
    "WinWidth",
    "WinHeight",
    "FSAA",
    "BPP",
    "DepthBufferDepth",
    "StencilBufferDepth",
    "ClearColor",
    "Fullscreen",
    "WindowsResizable",
    "ChildWindow",
    "PollingMode",
    "DefaultFont",
    "RenderMode",
    "PluginDllName"
};

Settings::Settings() :
        mPropertyMap(new Properties::BasePropertyMap) {
    initDefaults();
}

Settings::~Settings() {
    delete mPropertyMap;
}

bool Settings::configure(const String &initScriptFile) {
    String filename(initScriptFile);

    return true;
}

bool Settings::setString(Settings::ConfigKey key, const String &strValue) {
    if (strValue.empty() || key >= MaxKonfigKey || key < 0) {
        return false;
    }

    cppcore::Variant value;
    value.setStdString(strValue);
    mPropertyMap->setProperty(static_cast<ui32>(key), "", value);

    return true;
}

String Settings::getString(ConfigKey key) const {
    const cppcore::Variant &v = get(key);
    return v.getString();
}

bool Settings::setInt(ConfigKey key, i32 intValue) {
    if (key >= MaxKonfigKey) {
        return false;
    }

    cppcore::Variant value;
    value.setInt(intValue);
    mPropertyMap->setProperty(key, "", value);

    return true;
}

i32 Settings::getInt(ConfigKey key) const {
    const cppcore::Variant &v = get(key);
    return v.getInt();
}

bool Settings::setBool(ConfigKey key, bool val) {
    if (key >= MaxKonfigKey) {
        return false;
    }

    cppcore::Variant value;
    value.setBool(val);
    mPropertyMap->setProperty(key, "", value);

    return true;
}

bool Settings::getBool(ConfigKey key) const {
    const cppcore::Variant &v = get(key);
    return v.getBool();
}

bool Settings::setFloat(ConfigKey key, f32 floatValue) {
    if (key >= MaxKonfigKey) {
        return false;
    }
    cppcore::Variant value;
    value.setFloat(floatValue);
    mPropertyMap->setProperty(key, "", value);

    return true;
}

f32 Settings::getFloat(ConfigKey key) const {
    const cppcore::Variant &v = get(key);
    return v.getFloat();
}

const cppcore::Variant &Settings::get(ConfigKey key) const {
    Properties::Property *pProperty = mPropertyMap->getProperty(key);
    if (!pProperty) {
        return EmptyVariant;
    }

    return pProperty->getValue();
}

String Settings::getKeyAsString(ConfigKey key) const {
    const String stringKey(ConfigKeyStringTable[key]);
    return stringKey;
}

void Settings::clear() {
    if (nullptr != mPropertyMap) {
        mPropertyMap->clear();
    }
}

static i32 mapPlatformtype2Int(PluginType type) {
#ifdef OSRE_WINDOWS
    if (PluginType::WindowsPlugin == type) {
        return 0;
    }
#else
    if (PluginType::SDL2Plugin == type) {
        return 1;
    }
#endif
    return -1;
}

void Settings::initDefaults() {
    cppcore::Variant appName, windowsTitle, renderAPI, value;
    appName.setStdString("OSRE Application");
    mPropertyMap->setProperty(AppName, ConfigKeyStringTable[AppName], appName);

    value.setInt(static_cast<i32>(GfxApp));
    mPropertyMap->setProperty(AppType, ConfigKeyStringTable[AppName], value);

    // Set version 0.1.0 as default
    value.setInt(0);
    mPropertyMap->setProperty(AppVersionMajor, ConfigKeyStringTable[AppVersionMajor], value);
    value.setInt(1);
    mPropertyMap->setProperty(AppVersionMajor, ConfigKeyStringTable[AppVersionMinor], value);
    value.setInt(0);
    mPropertyMap->setProperty(AppVersionPatch, ConfigKeyStringTable[AppVersionPatch], value);

    windowsTitle.setStdString("The OSRE experience");
    const i32 pluginType( mapPlatformtype2Int( Platform::PlatformInterface::getOSPluginType() ) );
    value.setInt( pluginType );
    mPropertyMap->setProperty( WindowsTitle, ConfigKeyStringTable[ WindowsTitle ], windowsTitle );
    renderAPI.setStdString("opengl");
    mPropertyMap->setProperty( RenderAPI, ConfigKeyStringTable[ RenderAPI ], renderAPI );

    value.setInt( 0 );
    mPropertyMap->setProperty( WinX, ConfigKeyStringTable[ WinX ], value );
    mPropertyMap->setProperty( WinY, ConfigKeyStringTable[ WinY ], value );
    value.setInt( 1024 );
    mPropertyMap->setProperty( WinWidth, ConfigKeyStringTable[ WinWidth ], value );
    value.setInt( 768 );
    mPropertyMap->setProperty( WinHeight, ConfigKeyStringTable[ WinHeight ], value );

    value.setBool( true );
    mPropertyMap->setProperty( WindowsResizable, ConfigKeyStringTable[ WindowsResizable ], value );
    value.setBool( false );
    mPropertyMap->setProperty( ChildWindow, ConfigKeyStringTable[ ChildWindow ], value );
    value.setInt( 2 );
    mPropertyMap->setProperty( FSAA, ConfigKeyStringTable[ FSAA ], value );
    value.setInt( 32 );
    mPropertyMap->setProperty( BPP, ConfigKeyStringTable[ BPP ], value );
    value.setInt( 24 );
    mPropertyMap->setProperty( DepthBufferDepth, ConfigKeyStringTable[ DepthBufferDepth ], value );
    mPropertyMap->setProperty( StencilBufferDepth, ConfigKeyStringTable[ StencilBufferDepth ], value ); 
    
    value.setFloat4( 0, 0,0 ,0 );
    mPropertyMap->setProperty( ClearColor, ConfigKeyStringTable[ ClearColor ], value );
    value.setBool( false );
    mPropertyMap->setProperty( PollingMode, ConfigKeyStringTable[ PollingMode ], value );

    value.setStdString( "buildin_arial.bmp" );
    mPropertyMap->setProperty( DefaultFont, ConfigKeyStringTable[ DefaultFont ], value );

    value.setInt( 1 );
    mPropertyMap->setProperty( RenderMode, ConfigKeyStringTable[ RenderMode], value );
}

} // Namespace Properties
} // Namespace OSRE
