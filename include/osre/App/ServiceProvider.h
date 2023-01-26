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

#include <osre/App/AppCommon.h>

namespace OSRE {

// Forward declarations ---------------------------------------------------------------------------
namespace Common {
    class AbstractService;
}

namespace App {

/// @brief 
enum class ServiceType {
    InvalidService = -1,
    
    RenderService = 0,
    IOService,
    ResourceService,
    
    NumServices,
};

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT ServiceProvider {
public:
    static ServiceProvider *create();
    static void destroy();
    static void setService(ServiceType type, Common::AbstractService *service);
    template<class T>
    static T *getService(ServiceType type) {
        if (type == ServiceType::InvalidService || type == ServiceType::NumServices) {
            return nullptr;
        }
        return (T *)s_instance->mServiceArray[static_cast<size_t>(type)];
    }

private:
    explicit ServiceProvider();
    ~ServiceProvider() = default;

private:
    static ServiceProvider *s_instance;

    using ServiceArray = cppcore::TArray<Common::AbstractService*>;
    ServiceArray mServiceArray;
};

} // Namespace App
} // Namespace OSRE
