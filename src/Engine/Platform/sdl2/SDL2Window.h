/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2022 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include "Platform/AbstractWindow.h"

// Forward declarations ---------------------------------------------------------------------------
struct SDL_Window;

namespace OSRE::Platform {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class implements a SDL2-based window.
//-------------------------------------------------------------------------------------------------
class SDL2Surface final : public AbstractWindow {
public:
    /// @brief The class constructor.
    /// @param[in] props    The windows title.
    SDL2Surface(guid id, WindowsProperties *props );

    ///	@brief  The class destructor.
    ~SDL2Surface() override = default;

    /// @brief Will return a pointer showing to the SDL_Window.
    /// @return The sdl surface pointer.
    SDL_Window *getSDLSurface() const;

    /// @brief Will set a new windows title.
    /// @param[in] title    The new windows title.
    void setWindowsTitle( const String &title ) override;
    
    /// @brief  Will set the mouse cursor type.
    /// @param[in] ct   The new cursor type.
    void setWindowsMouseCursor(DefaultMouseCursorType ct) override;

    /// @brief  Will set the show window state.
    /// @param[in]  showState The new show state.
    void showWindow(ShowState showState) override;
    
protected:
    /// The onCreate callback implementation.
    virtual bool onCreate() override;
    /// The onDestroy callback implementation.
    virtual bool onDestroy() override;
    /// The onUpdateProperies callback implementation.
    virtual bool onUpdateProperies() override;
    /// The onResize handler.
    virtual void onResize( ui32 x, ui32 y, ui32 w, ui32 h ) override;

private:
    SDL_Window *mSurface;
};

} // Namespace Platform
// Namespace OSRE
