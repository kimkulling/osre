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
#pragma once

#include "Common/Object.h"
#include "Common/AbstractEventHandler.h>"

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace App {
    class AppBase;
}

namespace Modules {

class IModuleView;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
/// @brief
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT ModuleBase : public Common::Object, public Common::AbstractEventHandler {
public:
    virtual ~ModuleBase() = default;
    virtual void setModulelView(IModuleView *view);
    virtual IModuleView *getModuleView() const; 
    virtual bool load();
    virtual bool unload();
    virtual void update();
    virtual void render();
    virtual App::AppBase *getParentApp() const;
    virtual bool onEvent( const Common::Event &ev, const Common::EventData *eventData );

protected:
    ModuleBase(const String &name, App::AppBase *parentApp);
    virtual bool onLoad();
    virtual bool onUnload();
    virtual void onUpdate();
    virtual void onRender();
    virtual bool onAttached(const Common::EventData *eventData);
    virtual bool onDetached(const Common::EventData *eventData);


private:
    enum class ModuleState {
        Init,
        Loaded,
        Unloaded,
        Error
    } mState;
    IModuleView *mView;
    App::AppBase *mParentApp;
};

} // namespace Modules
} // namespace OSRE

