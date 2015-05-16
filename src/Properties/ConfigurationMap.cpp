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
#include <osre/Properties/ConfigurationMap.h>
#include <osre/Properties/BasePropertyMap.h>
#include <osre/Properties/Property.h>
#include <osre/Platform/PlatformInterface.h>
#include <cppcore/Common/Variant.h>

namespace OSRE {
namespace Properties {

static const CPPCore::Variant EmptyVariant;
static const String ConfigKeyStringTable[ ConfigurationMap::MaxKonfigKey ] = {
    "AppName",
    "AppType",
    "PlatformPlugin",
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
    "PollingMode"
};

//--------------------------------------------------------------------------------------------------------------------
ConfigurationMap::ConfigurationMap() 
: m_pPropertyMap( nullptr ) {
    m_pPropertyMap = new Properties::BasePropertyMap;

    initDefaults();
}

//--------------------------------------------------------------------------------------------------------------------
ConfigurationMap::~ConfigurationMap() {
    delete m_pPropertyMap;
    m_pPropertyMap = nullptr;
}

//--------------------------------------------------------------------------------------------------------------------
bool ConfigurationMap::configure( const String &initScriptFile ) {
    String filename( initScriptFile );

    return true;
}

//--------------------------------------------------------------------------------------------------------------------
bool ConfigurationMap::setString( ConfigurationMap::ConfigKey key, const String &strValue ) {
    if( strValue.empty( ) || key >= MaxKonfigKey || key < 0 ) {
        return false;
    }

    CPPCore::Variant value;
    value.setStdString( strValue );
    m_pPropertyMap->setProperty( static_cast<ui32>( key ), "", value );

    return true;
}

//--------------------------------------------------------------------------------------------------------------------
bool ConfigurationMap::setInt( ConfigKey key, i32 intValue ) {
    if ( key >= MaxKonfigKey ) {
        return false;
    }

    CPPCore::Variant value;
    value.setInt( intValue );
    m_pPropertyMap->setProperty( key, "", value );

    return true;
}

//--------------------------------------------------------------------------------------------------------------------
bool ConfigurationMap::setBool( ConfigKey key, bool val ) {
    if ( key >= MaxKonfigKey ) {
        return false;
    }

    CPPCore::Variant value;
    value.setBool( val );
    m_pPropertyMap->setProperty( key, "", value );

    return true;
}

//--------------------------------------------------------------------------------------------------------------------
bool ConfigurationMap::setFloat( ConfigKey key, f32 floatValue ) {
    if ( key >= MaxKonfigKey ) {
        return false;
    }
    CPPCore::Variant value;
    value.setFloat( floatValue );
    m_pPropertyMap->setProperty( key, "", value );

    return true;
}

//--------------------------------------------------------------------------------------------------------------------
const CPPCore::Variant &ConfigurationMap::get( ConfigKey key ) const {
    Properties::Property *pProperty = m_pPropertyMap->getProperty( key );
    if( !pProperty ) {
        return EmptyVariant;
    }

    return pProperty->getValue();
}

//--------------------------------------------------------------------------------------------------------------------
const String &ConfigurationMap::getKeyAsString( ConfigKey key ) const {
    return ConfigKeyStringTable[ key ];
}

//--------------------------------------------------------------------------------------------------------------------
void ConfigurationMap::clear() {
    if ( nullptr != m_pPropertyMap ) {
        m_pPropertyMap->clear();
    }
}

//--------------------------------------------------------------------------------------------------------------------
void ConfigurationMap::initDefaults() {
    CPPCore::Variant appName, windowsTitle, renderAPI, value;
    appName.setString( "ZFXCE2 Application" );
    m_pPropertyMap->setProperty( AppName, ConfigKeyStringTable[ AppName ], appName );
    
    value.setInt( static_cast<i32>( GfxApp ) );
    m_pPropertyMap->setProperty( AppType, ConfigKeyStringTable[ AppName ], value );

    windowsTitle.setString( "The ZFXCE2 experience" );
    value.setInt( Platform::PlatformInterface::getOSPluginType( ) );
    m_pPropertyMap->setProperty( PlatformPlugin, ConfigKeyStringTable[ PlatformPlugin  ], value );
    m_pPropertyMap->setProperty( WindowsTitle, ConfigKeyStringTable[ WindowsTitle ], windowsTitle );
    renderAPI.setString( "opengl" );
    m_pPropertyMap->setProperty( RenderAPI, ConfigKeyStringTable[ RenderAPI ], renderAPI );

    value.setInt( 0 );
    m_pPropertyMap->setProperty( WinX, ConfigKeyStringTable[ WinX ], value );
    m_pPropertyMap->setProperty( WinY, ConfigKeyStringTable[ WinY ], value );
    value.setInt( 800 );
    m_pPropertyMap->setProperty( WinWidth, ConfigKeyStringTable[ WinWidth ], value );
    value.setInt( 600 );
    m_pPropertyMap->setProperty( WinHeight, ConfigKeyStringTable[ WinHeight ], value );
    
    value.setInt( 2 );
    m_pPropertyMap->setProperty( FSAA, ConfigKeyStringTable[ FSAA ], value );
    value.setInt( 16 );
    m_pPropertyMap->setProperty( BPP, ConfigKeyStringTable[ BPP ], value );
    m_pPropertyMap->setProperty( DepthBufferDepth, ConfigKeyStringTable[ DepthBufferDepth ], value );
    m_pPropertyMap->setProperty( StencilBufferDepth, ConfigKeyStringTable[ StencilBufferDepth ], value ); 
    
    value.setFloat4( 0, 0,0 ,0 );
    m_pPropertyMap->setProperty( ClearColor, ConfigKeyStringTable[ ClearColor ], value );
    value.setBool( false );
    m_pPropertyMap->setProperty( PollingMode,ConfigKeyStringTable[ PollingMode ], value );
}

//--------------------------------------------------------------------------------------------------------------------

} // Namespace Properties
} // Namespace OSRE
