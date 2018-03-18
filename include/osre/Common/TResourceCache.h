/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/Logger.h>

namespace OSRE {
namespace Common {

static const c8 Tag[] = "TResourceCache";

template<class TResId, class TResource>
class TResourceCache {
public:
    TResourceCache();
    ~TResourceCache();
    void load( TResId id, const String &filename );
    TResource &get( TResId id );
    TResource &get( TResId id ) const;
    void clear();

private:
    using ResourceMap = std::map<TResId, std::unique_ptr<TResource>>;
    ResourceMap mResourceMap;
};

template<class TResId, class TResource>
inline
TResourceCache<TResId, TResource>::TResourceCache()
    : mResourceMap() {
    // empty
}

template<class TResId, class TResource>
inline
TResourceCache<TResId, TResource>::~TResourceCache() {
    clear();
}

template<class TResId, class TResource>
inline
void TResourceCache<TResId, TResource >::load( TResId id, const String &filename ) {
    if ( filename.empty() ) {
        osre_warn( Tag, "Filename is empty." );
        return;
    }

    std::unique_ptr<TResource> texPtr( new TResource );
    bool res = texPtr->loadFromFile( filename );
    if ( res ) {
        auto inserted = mResourceMap.insert( std::make_pair( id, std::move( texPtr ) ) );
        assert( inserted.second );
    } else {
        osre_warn( Tag, "Cannot load texture " + filename );
    }
}

template<class TResId, class TResource>
inline
TResource &TResourceCache<TResId, TResource>::get( TResId id ) {
    auto found = mResourceMap.find( id );
    return *found->second;
}

template<class TResId, class TResource>
inline
TResource &TResourceCache<TResId, TResource>::get( TResId id ) const {
    auto found = mResourceMap.find( id );
    return *found->second;
}

template<class TResId, class TResource>
inline
void TResourceCache<TResId, TResource>::clear() {
    mResourceMap.clear();
}

}
}
