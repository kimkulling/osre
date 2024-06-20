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

#include "Common/Logger.h"
#include "Common/osre_common.h"
#include "IO/Uri.h"
#include <map>

namespace OSRE {

namespace Common {

static const c8 *ResTag = "TResourceCache";

template <class TResource>
class TResourceFactory {
public:
    TResourceFactory();
    virtual ~TResourceFactory() = default;
    virtual TResource *create(const String &name, const IO::Uri &uri);
};

template <class TResource>
inline TResourceFactory<TResource>::TResourceFactory() {
    // empty
}

template <class TResource>
inline TResource *TResourceFactory<TResource>::create(const String &name, const IO::Uri &uri) {
    if (name.empty()) {
        osre_debug(ResTag, "Name is empty.");
        return nullptr;
    }

    TResource *res = new TResource(name, uri);

    return res;
}

template <class TResourceFactory, class TResource>
class TResourceCache {
public:
    TResourceCache();
    ~TResourceCache();
    void registerFactory(TResourceFactory &factory, bool owning);
    TResource *create(const String &name, const IO::Uri &uri = IO::Uri());
    TResource *find(const String &name) const;
    void clear();

private:
    using ResourceMap = std::map<String, TResource*>;
    ResourceMap m_resourceMap;
    TResourceFactory *m_factory;
    bool m_owner;
};

template <class TResourceFactory, class TResource>
inline TResourceCache<TResourceFactory, TResource>::TResourceCache() :
        m_resourceMap(), m_factory(new TResourceFactory), m_owner(true) {
    // empty
}

template <class TResourceFactory, class TResource>
inline TResourceCache<TResourceFactory, TResource>::~TResourceCache() {
    clear();
    if (m_owner) {
        delete m_factory;
    }
    m_factory = nullptr;
}

template <class TResourceFactory, class TResource>
inline void TResourceCache<TResourceFactory, TResource>::registerFactory(TResourceFactory &factory, bool owning) {
    if (nullptr != m_factory) {
        if (m_owner) {
            delete m_factory;
            m_owner = false;
        }
    }
    m_factory = &factory;
    m_owner = owning;
}

template <class TResourceFactory, class TResource>
inline TResource *TResourceCache<TResourceFactory, TResource>::create(const String &name, const IO::Uri &uri) {
    TResource *resource = m_factory->create(name, uri);
    if (nullptr == resource) {
        return nullptr;
    }
    m_resourceMap[name] = resource;

    return resource;
}

template <class TResourceFactory, class TResource>
inline TResource *TResourceCache<TResourceFactory, TResource>::find(const String &name) const {
    if (name.empty()) {
        return nullptr;
    }

    typename ResourceMap::const_iterator it = m_resourceMap.find(name);
    if (m_resourceMap.end() == it) {
        return nullptr;
    }

    return it->second;
}

template <class TResourceFactory, class TResource>
inline void TResourceCache<TResourceFactory, TResource>::clear() {
    typename ResourceMap::iterator it(m_resourceMap.begin());
    for (; it != m_resourceMap.end(); ++it) {
        delete it->second;
    }
    m_resourceMap.clear();
}

} // Namespace Common
} // Namespace OSRE
