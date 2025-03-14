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

#include "Common/Object.h"
#include "Common/osre_common.h"
#include "IO/Uri.h"

namespace OSRE::Common {

struct ResourceStatistics {
    size_t m_memory;

    ResourceStatistics() :
            m_memory(0) {
        // empty
    }
};

enum class ResourceState {
    Uninitialized = -1,
    Unloaded,
    Loaded,
    Error,
    Count
};

template <class TResType, class TResLoader>
class TResource : public Object {
public:
    TResource(const String &name, const IO::Uri &uri);
    virtual ~TResource() = default;
    void setUri(const IO::Uri &uri);
    const IO::Uri &getUri() const;
    ResourceState getState() const;
    virtual ResourceState load(TResLoader &loader);
    virtual ResourceState unload(TResLoader &loader);
    TResType *getRes();
    void setRes(TResType *res);

protected:
    TResType *create(const String &name);
    ResourceStatistics &getStats();
    virtual void setState(ResourceState newState);
    virtual ResourceState onLoad(const IO::Uri &uri, TResLoader &loader) = 0;
    virtual ResourceState onUnload(TResLoader &loader) = 0;

private:
    ResourceState mState;
    ResourceStatistics mStats;
    IO::Uri mUri;
    TResType *mRes;
};

template <class TResType, class TResLoader>
inline TResource<TResType, TResLoader>::TResource(const String &name, const IO::Uri &uri) :
        Object(name),
        mState(ResourceState::Uninitialized),
        mUri(uri),
        mRes(nullptr) {
    // empty
}

template <class TResType, class TResLoader>
inline void TResource<TResType, TResLoader>::setUri(const IO::Uri &uri) {
    if (mUri == uri) {
        return;
    }

    mUri = uri;
}

template <class TResType, class TResLoader>
inline const IO::Uri &TResource<TResType, TResLoader>::getUri() const {
    return mUri;
}

template <class TResType, class TResLoader>
inline ResourceState TResource<TResType, TResLoader>::getState() const {
    return mState;
}

template <class TResType, class TResLoader>
inline TResType *TResource<TResType, TResLoader>::create(const String &name) {
    mRes = new TResType(name);
    return mRes;
}

template <class TResType, class TResLoader>
inline TResType *TResource<TResType, TResLoader>::getRes() {
    return mRes;
}

template<class TResType, class TResLoader>
inline void TResource<TResType, TResLoader>::setRes(TResType *res) {
    mRes = res;
}

template <class TResType, class TResLoader>
inline ResourceStatistics &TResource<TResType, TResLoader>::getStats() {
    return mStats;
}

template <class TResType, class TResLoader>
inline ResourceState TResource<TResType, TResLoader>::load(TResLoader &loader) {
    return onLoad(getUri(), loader);
}

template <class TResType, class TResLoader>
inline ResourceState TResource<TResType, TResLoader>::unload(TResLoader &loader) {
    return onUnload(loader);
}

template <class TResType, class TResLoader>
inline void TResource<TResType, TResLoader>::setState(ResourceState newState) {
    mState = newState;
}

} // Namespace OSRE::Common
