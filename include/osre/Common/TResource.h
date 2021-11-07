/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/Object.h>
#include <osre/Common/osre_common.h>
#include <osre/IO/Uri.h>

namespace OSRE {
namespace Common {

/// @brief Data collection about resources in use.
struct ResourceStatistics {
    size_t m_memory;    ///< Memory usage.

    /// @brief The default class constructor.
    ResourceStatistics() :
            m_memory(0) {
        // empty
    }
};

///	@brief  This enum is used to describe the resource state.
/// Uninitialized -> Unloaded
///                      | |   -> Error
///                  Loaded
enum class ResourceState {
    Uninitialized,  ///< Uninitialized.
    Unloaded,       ///< Initialized, but not loaded.
    Loaded,         ///< Loaded.
    Error           ///< An error has occurred.
};

/// @brief This template class is used to define resources.
/// @tparam TResType   The resource type. 
/// @tparam TResLoader The resource loader class.
template <class TResType, class TResLoader>
class TResource : public Object {
public:
    /// @brief The class constructor.
    /// @param[in] name     The resource name
    /// @param[in] uri      The uri to load the resource.
    TResource(const String &name, const IO::Uri &uri);

    /// @brief The class destructor, virtual.
    virtual ~TResource();

    /// @brief Will assign a new uri, resource state will unloaded.
    /// @param[in] uri  The new uri.
    virtual void setUri(const IO::Uri &uri);
    
    /// @brief  Will return the uri.
    /// @return The uri.
    virtual const IO::Uri &getUri() const;
    
    /// @brief  Will return the resource state.
    /// @return The resource state.
    virtual ResourceState getState() const;
    
    /// @brief Loads the resource.
    /// @param[in] loader 
    /// @return 
    virtual ResourceState load(TResLoader &loader);

    /// @brief  Unloads the resource.
    /// @param[in] loader   The resource loader.
    /// @return     The new resource state.
    virtual ResourceState unload(TResLoader &loader);
    
    /// @brief  Will return the resource instance.
    /// @return The resource instance.
    virtual TResType *get();

protected:
    TResType *create();
    ResourceStatistics &getStats();
    virtual void setState(ResourceState newState);
    virtual ResourceState onLoad(const IO::Uri &uri, TResLoader &loader) = 0;
    virtual ResourceState onUnload(TResLoader &loader) = 0;

private:
    ResourceState m_state;
    ResourceStatistics m_stats;
    IO::Uri m_uri;
    TResType *m_res;
};

template <class TResType, class TResLoader>
inline TResource<TResType, TResLoader>::TResource(const String &name, const IO::Uri &uri) :
        Object(name),
        m_state(ResourceState::Uninitialized),
        m_stats(),
        m_uri(uri),
        m_res(nullptr) {
    // empty
}

template <class TResType, class TResLoader>
inline TResource<TResType, TResLoader>::~TResource() {
    // empty
}

template <class TResType, class TResLoader>
inline void TResource<TResType, TResLoader>::setUri(const IO::Uri &uri) {
    if (m_uri == uri) {
        return;
    }

    m_uri = uri;
}

template <class TResType, class TResLoader>
inline const IO::Uri &TResource<TResType, TResLoader>::getUri() const {
    return m_uri;
}

template <class TResType, class TResLoader>
inline ResourceState TResource<TResType, TResLoader>::getState() const {
    return m_state;
}

template <class TResType, class TResLoader>
inline TResType *TResource<TResType, TResLoader>::create() {
    m_res = new TResType;
    return m_res;
}

template <class TResType, class TResLoader>
inline TResType *TResource<TResType, TResLoader>::get() {
    return m_res;
}

template <class TResType, class TResLoader>
inline ResourceStatistics &TResource<TResType, TResLoader>::getStats() {
    return m_stats;
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
    m_state = newState;
}

} // Namespace Common
} // Namespace OSRE
