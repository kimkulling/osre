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

#include "Common/Logger.h"
#include "Common/osre_common.h"
#include "IO/Uri.h"

#include <map>

namespace OSRE::Common {

static constexpr c8 ResTag[] = "TResourceCache";

template <class TResource>
class TResourceFactory {
public:
    TResourceFactory() = default;
    virtual ~TResourceFactory() = default;
    virtual TResource *create(const String &name, const IO::Uri &uri);
};

template <class TResource>
inline TResource *TResourceFactory<TResource>::create(const String &name, const IO::Uri &uri) {
    if (name.empty()) {
        osre_debug(ResTag, "Name is empty.");
        return nullptr;
    }

    auto *res = new TResource(name, uri);

    return res;
}

template <class TResourceFactory, class TResource>
class TResourceCache {
public:
    TResourceCache();
    ~TResourceCache();
    void registerFactory(TResourceFactory &factory, bool owning);
    TResource *create(const String &name, const IO::Uri &uri = IO::Uri());
    void set(const String &name, TResource *resource);
    TResource *find(const String &name) const;
    void clear();

private:
    using ResourceMap = std::map<String, TResource*>;
    ResourceMap mResourceMap;
    TResourceFactory *mFactory;
    bool mOwner;
};

template <class TResourceFactory, class TResource>
inline TResourceCache<TResourceFactory, TResource>::TResourceCache() :
        mResourceMap(), mFactory(new TResourceFactory), mOwner(true) {
    // empty
}

template <class TResourceFactory, class TResource>
inline TResourceCache<TResourceFactory, TResource>::~TResourceCache() {
    clear();
    if (mOwner) {
        delete mFactory;
    }
}

template <class TResourceFactory, class TResource>
inline void TResourceCache<TResourceFactory, TResource>::registerFactory(TResourceFactory &factory, bool owning) {
    if (nullptr != mFactory) {
        if (mOwner) {
            delete mFactory;
            mOwner = false;
        }
    }
    mFactory = &factory;
    mOwner = owning;
}

template <class TResourceFactory, class TResource>
inline TResource *TResourceCache<TResourceFactory, TResource>::create(const String &name, const IO::Uri &uri) {
    TResource *resource = mFactory->create(name, uri);
    if (nullptr == resource) {
        return nullptr;
    }
    mResourceMap[name] = resource;

    return resource;
}

template <class TResourceFactory, class TResource>
inline TResource *TResourceCache<TResourceFactory, TResource>::find(const String &name) const {
    if (name.empty()) {
        return nullptr;
    }

    typename ResourceMap::const_iterator it = mResourceMap.find(name);
    if (mResourceMap.end() == it) {
        return nullptr;
    }

    return it->second;
}

template <class TResourceFactory, class TResource>
inline void TResourceCache<TResourceFactory, TResource>::set(const String &name, TResource *resource) {
    if (name.empty()) {
        return;
    }
    auto it = mResourceMap.find(name);
    if (it != mResourceMap.end()) {
        delete it->second;
    }
    mResourceMap[name] = resource;
}

template <class TResourceFactory, class TResource>
inline void TResourceCache<TResourceFactory, TResource>::clear() {
    typename ResourceMap::iterator it(mResourceMap.begin());
    for (; it != mResourceMap.end(); ++it) {
        delete it->second;
    }
    mResourceMap.clear();
}

} // Namespace OSRE::Common
