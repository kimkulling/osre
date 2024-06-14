#pragma once

#include <osre/Common/osre_common.h>
#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace App {

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
