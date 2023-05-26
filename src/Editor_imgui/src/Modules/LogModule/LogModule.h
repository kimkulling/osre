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
#pragma once

#include <osre/Modules/ModuleBase.h>

namespace OSRE {

namespace Common {
    class AbstractLogStream;    
}

namespace Editor {

class LogView;
class AssimpLogStream;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
/// @brief  This module implements the logic and the UI fr the log output.
//-------------------------------------------------------------------------------------------------
class LogModule : public Modules::ModuleBase {
public:
    /// @brief The class constructor.
    /// @param[in] parentApp    The parent application.
    LogModule(App::AppBase *parentApp);

    ///	@brief  The class destructor.
    ~LogModule() override;

protected:
    /// @brief The onLoad callback handler.
    /// @return true if no error has occurred.
    bool onLoad() override;

    /// @brief The onUnload callback handler.
    /// @return true if no error has occurred.
    bool onUnload() override;

    /// @brief The onUpdate callback handler, all log output will be written here.
    void onUpdate() override;

private:
    LogView *mLogView;
    Common::AbstractLogStream *mLogStream;
    AssimpLogStream *mAssimpLogStream;
};

} // namespace Editor
} // namespace OSRE
