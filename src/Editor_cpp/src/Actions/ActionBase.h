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

#include <osre/Common/osre_common.h>
#include <cppcore/Container/TArray.h>
#include <cppcore/Common/Variant.h>

namespace OSRE {
namespace Editor {

using ArgumentList = ::CPPCore::TArray<::CPPCore::Variant*>;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Editor
///
///	@brief  This class declares the base interface for all actions.
//-------------------------------------------------------------------------------------------------
class ActionBase {
public:
    /// @brief The class destructor.
    virtual ~ActionBase() = default;

    /// @brief The runner.
    /// @param args     Argument list for the action run.
    /// @return The result.
    virtual bool run(const ArgumentList &args);

protected:
    /// @brief The default class constructor.
    ActionBase() = default;

    /// @brief The on run method, override this for your own action runnings.
    /// @param args     Argument list
    /// @return The result, true for success, false for a failure.
    virtual bool onRun(const ArgumentList &args) = 0;
};

inline bool ActionBase::run(const ArgumentList &args) {
    const bool result = onRun(args);
    return result;
}

} // namespace Editor
} // namespace OSRE
