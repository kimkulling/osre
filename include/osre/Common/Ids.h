/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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

namespace OSRE {
namespace Common {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class the API for id container. You can use an id container to store unique ids.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Ids {
public:
    /// @brief  The default class constructor.
    Ids();

    /// @brief  The class constructor with the start id.
    /// @param  startId     [in] The start id.
    Ids(ui64 startId);

    /// @brief  The class destructor.
    ~Ids();

    /// @brief  Returns an unique id.
    /// @return The unique id.
    ui64 getUniqueId();

    /// @brief  Will release a unique id.
    /// @param  id      [in] The unique id to release.
    void releaseId(ui64 id);

    // Copying is not allowed
    Ids(const Ids &) = delete;
    Ids &operator = (const Ids &) = delete;

private:
    CPPCore::TArray<ui64> m_freeIds;
    ui64 m_last;
};

} // Namespace Common
} // Namespace OSRE
