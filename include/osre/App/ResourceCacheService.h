#pragma once

#include <osre/common/TResourceCache.h>
#include <osre/RenderBackend//RenderCommon.h>
#include <osre/Common/TResource.h>

namespace OSRE {
namespace App {

using TextureResourceFactory = Common::TResourceFactory <RenderBackend::TextureResource> ;
using TextureResourceCache = Common::TResourceCache<TextureResourceFactory, RenderBackend::TextureResource>;

class ResourceCacheService {
public:
    ResourceCacheService();
    ~ResourceCacheService();
    TextureResourceCache *getTextureResourceCache() const;

private:
    TextureResourceCache *m_texResCache;
};

inline
ResourceCacheService::ResourceCacheService()
: m_texResCache( new TextureResourceCache ) {
    //
}

inline
ResourceCacheService::~ResourceCacheService() {
    delete m_texResCache;
    m_texResCache;
}

inline
TextureResourceCache *ResourceCacheService::getTextureResourceCache() const {
    return m_texResCache;
}

}
}
