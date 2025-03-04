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

#include "Common/AbstractService.h"
#include "Common/TResourceCache.h"
#include "RenderBackend/RenderCommon.h"

namespace OSRE::App {

using TextureResourceFactory = Common::TResourceFactory<RenderBackend::TextureResource>;
using TextureResourceCache = Common::TResourceCache<TextureResourceFactory, RenderBackend::TextureResource>;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
/// @brief The resource cache service.
//-------------------------------------------------------------------------------------------------
class ResourceCacheService : public Common::AbstractService {
public:
    /// @brief The class constructor.
    ResourceCacheService();

    /// @brief The class destructor.
    ~ResourceCacheService();

    /// @brief Will return thr texture resource cache.
    /// @return The resource cache for textures.
    TextureResourceCache *getTextureResourceCache() const;

protected:
    bool onOpen() override {
        return true;
    }
    bool onClose() override {
        return true;
    }
    bool onUpdate() override {
        return true;
    }

private:
    TextureResourceCache *m_texResCache;
};

inline ResourceCacheService::ResourceCacheService() :
        AbstractService("resoursecacheservice"),
        m_texResCache(new TextureResourceCache) {
    //
}

inline ResourceCacheService::~ResourceCacheService() {
    delete m_texResCache;
}

inline TextureResourceCache *ResourceCacheService::getTextureResourceCache() const {
    return m_texResCache;
}

} // namespace OSRE::App
