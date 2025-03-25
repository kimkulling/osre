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
#pragma once

#include "Common/osre_common.h"
#include "Common/Ids.h"
#include "App/App.h"

namespace OSRE {

//-------------------------------------------------------------------------------------------------
/// @ingroup    Samples
///
/// @brief Basic utilities for the samples.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT SampleAppBase {
public:
    /// @brief The class constructor.
    SampleAppBase() = default;

    /// @brief The class destructor.
    virtual
    ~SampleAppBase() = default;

    /// @brief Will initialize the camera for the scene.
    /// @param[in] name     The name for the camera entity.
    /// @param[in] scene    The scene to look at.
    /// @param[in] w        The width of the viewport.
    /// @paran[in] ids      Id container for guids.
    /// @param[in] h        The height of the viewport.
    /// @return The new created camera instance.
    static App::CameraComponent *setupCamera(const String &name, App::Scene *scene, ui32 w, ui32 h, Common::Ids &ids);
};

}