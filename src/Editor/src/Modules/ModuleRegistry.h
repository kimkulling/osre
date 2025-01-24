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
#include <cppcore/Container/TArray.h>

namespace OSRE {

namespace Modules {
    class ModuleBase;
}

namespace Editor {


/// @brief  The type to store modules in an array.
using ModuleArray = cppcore::TArray<Modules::ModuleBase*>;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
/// @brief  This class is the registry for all given modules.
//-------------------------------------------------------------------------------------------------
class ModuleRegistry {
public:
    /// @brief  Type to store the paths to the modules.
    using ModulePathArray = StringArray;

    /// @brief  The class constructor.
    ModuleRegistry();

    /// @brief  The class destructor.
    ~ModuleRegistry();

    /// @brief  Will add a search path.
    /// @param  path    [in] The path to add.
    void addPath(const String &path);

    /// @brief  Will register a new module.
    /// @param  mod     [in] The module to register.
    /// @return true if successful. False in case of an error.
    bool registerModule(Modules::ModuleBase *mod);

    /// @brief  Will search for a given module name.
    /// @param  name    [in] The module name to search for.
    /// @return The found module or a nullptr.
    Modules::ModuleBase *findModule(const String &name) const;
    
    /// @brief  Will unregister the given module.
    /// @param  mod     [in] The module to unregister.
    /// @return true if successful.
    bool unregisterModule(Modules::ModuleBase *mod);
    
    /// @brief  All registered module will get an update.
    void update();

    /// @brief  All registered module will get rendered.
    void render();

private:
    ModuleArray mModules;
    ModulePathArray mPaths;
};

} // Namespace Editor
} // Namespace OSRE
