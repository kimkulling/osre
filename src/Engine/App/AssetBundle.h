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

#include "Common/osre_common.h"

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace App {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This helper class can be used to bundle files.
//-------------------------------------------------------------------------------------------------
class AssetBundle {
public:
    /// @param  The class construtor.
    /// @param[in] name     The bundle name.
    AssetBundle(const String &name);

    /// @brief The class destructor.
    ~AssetBundle() = default;

    /// @brief Will validate the bundle validation.
    /// @return tue if it it valid, false if not.
    bool isValid() const;

    /// @brief Will return the bundle name.
    /// @return The bundle name.
    const String &getName() const;
    
    /// @brief Will add a file to the bundle.
    /// @param file     The file to add.
    void add(const String &file);

    /// @brief Will remove a file from the bundle.
    /// @param file     The file to remove.
    void remove(const String &file);
    
    /// @brief Will return the number of files in the bundle.
    /// @return The number of files in the bundle.
    size_t getNumAssets() const;

    /// @brief Will return the asset from the bundle at the given index.
    /// @param index    The asset index.
    /// @return The asset name.
    const String &getAssetAt(size_t index) const;

private:
    bool isSupported(const String &file) const;

    String mName;
    using AssetArray = cppcore::TArray<String>;
    AssetArray mAssetArray;
};

inline AssetBundle::AssetBundle(const String &name) : mName(name) {
    // empty
}

inline bool AssetBundle::isValid() const {
    return AssetBundle::isSupported(getName());
}

inline const String &AssetBundle::getName() const {
    return mName;
}

inline void AssetBundle::add(const String &file) {
    mAssetArray.add(file);
}

inline void AssetBundle::remove(const String &file) {
    AssetArray::Iterator i = mAssetArray.linearSearch(file);
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

inline bool AssetBundle::isSupported(const String &file) const {
    if (file.empty()) {
        return false;
    }

    String::size_type pos = file.find_last_of('.');
    if (pos == String::npos) {
        return false;
    }

    return false;
}

} // Namespace Assets
} // Namespace OSRE
