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
#include "App/ServiceProvider.h"
#include "RenderBackend/RenderBackendService.h"
#include "IO/IOService.h"

namespace OSRE::App {

using namespace ::OSRE::RenderBackend;
using namespace ::OSRE::Common;

ServiceProvider *ServiceProvider::sInstance = nullptr;

ServiceProvider *ServiceProvider::create() {
    if (nullptr == sInstance) {
        sInstance = new ServiceProvider;
    }

    return sInstance;
}

void ServiceProvider::destroy() {
    if (nullptr != sInstance) {
        delete sInstance;
        sInstance = nullptr;
    }
}

void ServiceProvider::setService(ServiceType type, Common::AbstractService *service) {
    if (type == ServiceType::Invalid || type == ServiceType::Count) {
    }
    sInstance->mServiceArray[static_cast<size_t>(type)] = service;
}

ServiceProvider::ServiceProvider() :
        mServiceArray() {
    mServiceArray.resize(static_cast<size_t>(ServiceType::Count));
    mServiceArray.set(nullptr);
}

} // namespace OSRE::App