/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2019 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/Object.h>
#include <osre/IO/Uri.h>

namespace OSRE {
namespace Common {

struct ResourceStatistics {
    size_t m_memory;

    ResourceStatistics()
    : m_memory(0) {
        // empty
    }
};

template<class TResType, class TResLoader>
class TResource : public Object {
public:
    enum ResourceState {
        Uninitialized,
        Unloaded,
        Loaded,
        Error
    };

    TResource(const String& name);
    virtual ~TResource();
    ResourceState getState() const;
    virtual void load( const IO::Uri& uri, TResLoader& loader);
    virtual void unload(TResLoader& loader);

protected:
    void create();
    TResType *get();
    ResourceStatistics& getStats();
    virtual void setState(ResourceState newState);
    virtual void onLoad(const IO::Uri& uri, TResLoader& loader) = 0;
    virtual void onUnload(TResLoader& loader) = 0;

private:
    ResourceState m_state;
    ResourceStatistics m_stats;
    TResType *m_res;
};

template<class TResType, class TResLoader>
inline
TResource<TResType, TResLoader>::TResource(const String& name)
: Object(name)
, m_state(Uninitialized)
, m_res( nullptr ) {
    ::memset(&m_stats, 0, sizeof(ResourceStatistics));
}

template<class TResType, class TResLoader>
inline
TResource<TResType, TResLoader>::~TResource() {
    // empty
}

template<class TResType, class TResLoader>
inline
typename TResource<TResType, TResLoader>::ResourceState TResource<TResType, TResLoader>::getState() const {
    return m_state;
}

template<class TResType, class TResLoader>
inline
void TResource<TResType, TResLoader>::create() {
    m_res = new TResType;
}

template<class TResType, class TResLoader>
inline
TResType* TResource<TResType, TResLoader>::get() {
    return m_res;
}

template<class TResType, class TResLoader>
inline
ResourceStatistics& TResource<TResType, TResLoader>::getStats() {
    return m_stats;
}

template<class TResType, class TResLoader>
inline
void TResource<TResType, TResLoader>::load(const IO::Uri& uri, TResLoader& loader) {
    onLoad(uri, loader);
}

template<class TResType, class TResLoader>
inline
void TResource<TResType, TResLoader>::unload(TResLoader& loader) {
    onUnload(loader);
}

template<class TResType, class TResLoader>
inline
void TResource<TResType, TResLoader>::setState(ResourceState newState) {
    m_state = newState;
}

} // Namespace Common
} // Namespace OSRE
