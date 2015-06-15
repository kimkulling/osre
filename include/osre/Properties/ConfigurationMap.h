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
#pragma once  

#include <osre/Common/osre_common.h>

#include <cppcore/Common/Variant.h>

namespace OSRE {
namespace Properties {

class IPropertyMap;

//--------------------------------------------------------------------------------------------------------------------
///	@class		ZFXCE2::Properties::ConfigurationMap
///	@ingroup	Infrastructure
///
///	@brief
//--------------------------------------------------------------------------------------------------------------------
class OSRE_EXPORT ConfigurationMap {
public:
    /// @enum   WorkingModeType
    ///	@brief  Enum to describe the requested working mode.
    enum WorkingModeType {
        ConsoleApp = 0,     ///< Console working mode, no rendering at all.
        GfxApp              ///< Rendering working mode.
    };
    
    ///	@enum	ConfigKey
    ///	@brief	The description of the startup property.
    enum ConfigKey {
        AppName = 0,			///< The application name.
        AppType,                ///< Describes the application type, UI or console.
        PlatformPlugin,         ///< The platform plugin type.
        WindowsTitle,			///< The title of the main window.
        RenderAPI,				///< The requested render API.
        WinX,					///< The x coordinate of the upper left window point.
        WinY,					///< The y coordinate of the upper left window point.
        WinWidth,				///< The width of the window.
        WinHeight,				///< The height of the window.
        FSAA,					///< Full-screen anti-aliasing sample mode.
        BPP,					///< The Color depth.
        DepthBufferDepth,		///< The Z-depth.
        StencilBufferDepth,		///< The stencil depth.
        ClearColor,				///< The clear color.
        FullScreen,             ///< The option for full screen.
        PollingMode,            ///< Polling mode, true for polling requested.
        MaxKonfigKey			///< The upper limit.
    };

public:
    ///	@brief	The default class constructor.
    ConfigurationMap();

    ///	@brief	The class destructor.
    ~ConfigurationMap();

    ///	@brief	The configuration map will be initiated by a given configuration file.
    ///	@param	initScriptFile		[in] The name of the init script file.
    ///	@return	true, if configuration was performed with success, false if not.
    bool configure( const String &initScriptFile );

    ///	@brief	Set a configuration key with the assigned value.
    ///	@param	key		The configuration key.
    ///	@param	rValue	The assigned value.
    ///	@return	true, of set was successful, false if not.
    bool setString( ConfigKey key, const String &rValue );
    bool setInt( ConfigKey key, i32 value );
    bool setBool( ConfigKey key, bool value );
    bool setFloat( ConfigKey key, f32 value );
    ///	@brief	Returns the value for the key.
    ///	@param	key	Key to look for.
    ///	@return	The assigned variant.
    const CPPCore::Variant &get( ConfigKey key ) const;
    
    const String &getKeyAsString( ConfigKey key ) const;

    ///	@brief	Clears the whole map.
    void clear();

protected:
    void initDefaults();

private: //	Copying is not allowed.
    ConfigurationMap( const ConfigurationMap & );
    ConfigurationMap &operator = ( const ConfigurationMap & );

private:
    Properties::IPropertyMap *m_pPropertyMap;
};

//--------------------------------------------------------------------------------------------------------------------

} // Namespace Properties
} // Namespace OSRE
