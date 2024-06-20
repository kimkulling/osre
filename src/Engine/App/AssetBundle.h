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

#include "Common/osre_common.h"

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace App {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	
//-------------------------------------------------------------------------------------------------
class AssetBundle {
public:
    AssetBundle(const String &name);
    ~AssetBundle() = default;
    bool isValid() const;
    const String &getName() const;
    bool isSupported(const String &file) const;
    void add(const String &file);
    void remove(const String &file);
    size_t getNumAssets() const;
    const String &getAssetAt(size_t index) const;

private:
    String mName;
    cppcore::TArray<String> mExtList;
    using AssetArray = cppcore::TArray<String>;
    AssetArray mAssetArray;
};

inline AssetBundle::AssetBundle(const String &name) : mName(name) {
    mExtList.add("zip");
}

inline bool AssetBundle::isValid() const {
    const String::size_type pos = mName.find_last_of(".");
    if (pos == String::npos) {
        return true;
    }

    return AssetBundle::isSupported(getName());
}

inline const String &AssetBundle::getName() const {
    return mName;
}

inline bool AssetBundle::isSupported(const String &file) const {
    if (file.empty()) {
        return false;
    }

    String::size_type pos = file.find_last_of('.');
    if (pos == String::npos) {
        return false;
    }

    String ext = file.substr(pos+1, file.size() - pos - 1);
    for (size_t i=0; i<mExtList.size(); ++i) {
        if (ext == mExtList[i]) {
            return true;
        }
    }

    return false;
}

inline void AssetBundle::add(const String &file) {
    mAssetArray.add(file);
}

inline void AssetBundle::remove(const String &file) {
    AssetArray::Iterator i = mAssetArray.find(file);
    if (i !=mAssetArray.end()) {
        mAssetArray.remove(i);
    }
}

inline size_t AssetBundle::getNumAssets() const {
    return mAssetArray.size();
}

inline const String &AssetBundle::getAssetAt(size_t index) const {
    return mAssetArray[index];
}

} // Namespace Assets
} // Namespace OSRE
