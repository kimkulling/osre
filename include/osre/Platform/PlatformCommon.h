/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kullingopy of
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
#include <osre/Common/Object.h>

namespace OSRE {

// Forward declarations
namespace Common {
    class EventTriggerer;
}

namespace Properties {
    class Settings;
}

namespace Platform {

// Forward declarations
class AbstractWindow;
class OSEventListener;
class AbstractThreadEvent;
class AbstractThreadLocalStorage;
class AbstractCriticalSection;
class AbstractThreadEvent;
class AbstractThread;
class AbstractAtomic;
class AbstractThreadLocalStorage;
class AbstractTimer;
class AbstractPlatformEventQueue;
class AbstractOGLRenderContext;
class AbstractDynamicLoader;
class AbstractSystemInfo;
class AbstractOSService;
class CPUInfo;
class PlatformInterface;

struct AbstractInputUpdate;
struct WindowsProperties;

} // Namespace Platform
} // Namespace OSRE

