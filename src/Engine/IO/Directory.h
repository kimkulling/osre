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

#include "IO/IOCommon.h"

namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class implements static helpers to work with directories more easily.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Directory {
public:
    using FileList = StringArray;

    /// @brief  Will return true, when the directory exists.
    /// @param  dir     [in] The name of the directory.
    /// @return true, when the directory exists.
    static bool exists( const String &dir );

    /// @brief  Will separate the directory- and filename from an absolute path.
    /// @param  absPath     [in] The absolute name with the path and the filename.
    /// @param              [out] The path.
    /// @param  filename    [out] The filename.
    static bool getDirectoryAndFile(const String &absPath, String &path, String &filename);

    ///	@brief	Returns the directory separator for the current platform.
    ///	@return	The directory separator 
    /// @remark For instance using a Unix platform / will be returned.
    static String getDirSeparator();

    ///	@brief	Will set the current working directory.
    /// @param  absPath     [in] The new directory.
    /// @return true, if the directory exists, false if not.
    static bool setCurrentDirectory(const String& absPath);

    /// @brief  Will return The current working directory.
    /// @return The current working directory.
    static String getCurrentDirectory();
    
    /// 
    static bool createDirectory( const c8 *name );

    ///
    static bool getFileList(const String &absPath, FileList &files);
};

} // Namespace IO
} // Namespace OSRE
