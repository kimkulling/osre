/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <cppcore/Container/THashMap.h>

namespace OSRE {
    
// Forward declarations ---------------------------------------------------------------------------
namespace IO {
    class Stream;
    class Uri;
}

namespace RenderBackend {
    struct BufferData;
}

namespace App {

class World;

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief  This class is a helper to resolve mounting points to valid path names in a file system.
/// If you want to assign a folder called /home/me/data to the asset folder you can use this to
/// register it to a mounting point asset:
/// 
/// @code
/// AssetRegistry::create();
/// AssetRegistry::registerAssetPath("/home/me/data", "asset");
/// ...
/// IO::Uri myUri("assets/data.obj")
/// String path = AssetRegistry::resolvePathFromeUri(myUri);
/// @endcode
/// 
/// This will return /home/me/data/data.obj
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT AssetRegistry {
public:
    /// @brief  Will create the only instance.
    /// @return The singleton instance as a pointer.
    static AssetRegistry *create();

    /// @brief Will destroy the only instance.
    static void destroy();

    /// @brief Registers a new asset path.
    /// @param[in] mount    The mount point.
    /// @param[in] path     The path to mount.
    /// @return true in case of success.
    static bool registerAssetPath( const String &mount, const String &path );

    /// @brief  Helper method to check if a mounting point was mounted before.
    /// @param[in] mount    The name of the mount point.
    /// @return true if is already mounted, false if not.
    static bool hasPath( const String &mount );

    /// @brief  Will return the assigned path to a given mounting point.
    /// @param[in] mount    The mount point to check.
    /// @return The assigned path or an empty string, if no mounting point was assigned to.
    static String getPath( const String &mount );

    /// @brief  Will return a given uri with a mounting point to a valid file system path.
    /// @param[in] location     The location to resolve.
    /// @return The valid path.
    static String resolvePathFromUri( const IO::Uri &location );

    /// @brief Will clear all data.
    /// @return false if not created before.
    static bool clear();

private:
    /// @brief The default class constructor.
    AssetRegistry();

    /// @brief The class destructor.
    ~AssetRegistry();

private:
    static AssetRegistry *s_instance;

    using Name2PathMap = cppcore::THashMap<HashId, String>;
    Name2PathMap m_name2pathMap;
};

} // Namespace Assets
} // Namespace OSRE
