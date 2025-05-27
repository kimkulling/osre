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

namespace OSRE::IO {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class represents a URI ( see
///	http://en.wikipedia.org/wiki/Uniform_Resource_Identifier ). You can use it to describe the
///	location of resources like:
///
///	file://c:/texture.jpg
///
/// For this example the schema is: file.
/// For this example the path is c:/.
/// For this example the resource name is texture.jpg.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Uri final {
public:
    /// @brief  Enum to desribe the schema type.
    enum SchemeType {
        Invalid = -1,   ///< Invalid scheme
        FileScheme,     ///< File scheme
        Count           ///< Number of supported schemes
    };

    ///	@brief	The default class constructor.
    Uri() = default;

    ///	@brief	The class constructor with a string containing a valid URI. If the string description
    ///			is not ok the URI representation will be empty.
    ///	@param[in] uri      The string with the URI.
    explicit Uri(const String &uri);

    ///	@brief	The class destructor.
    ~Uri() = default;

    /// @brief  Will construct a valid uri from the given scheme, path and resource name.
    /// @param[in] scheme   The scheme to use.
    /// @param[in] path     The path to the resource.
    /// @param[in] res      The resource name.
    /// @return The uri string.
    static String constructFromComps(SchemeType type, const String &path, const String &res);

    static String schemeEnumToStr(SchemeType type);

    ///	@brief	Assigns a new uri string, the older data will be erased.
    ///	@param[in] uri      The string with the URI.
    void setUri(const String &uri);

    ///	@brief	Returns the uri string.
    ///	@return	The uri string.
    const String &getUri() const;

    ///	@brief	Returns the scheme of the URI.
    ///	@return	A string containing the scheme.
    SchemeType getScheme() const;

    /// @brief  Will assign a new scheme description.
    /// @param[in] scheme   The new scheme.
    void setScheme(SchemeType scheme);

    ///	@brief	Returns the path of the URI.
    ///	@return	A string containing the path.
    const String &getPath() const;

    /// @brief  Will set a new path.
    /// @param[in] path     The new path to use.
    void setPath( const String &path );

    ///	@brief	Returns the resource name of the URI.
    ///	@return	A string containing the absolute path.
    const String &getAbsPath() const;

    ///	@brief	Returns the absolute path ( path + resource ) of the URI.
    ///	@return	A string containing the resource.
    const String &getResource() const;

    /// @brief  Will set the resource string.
    /// @param[in] resName      The name of the new resource.
    void setResource( const String &resName );

    ///	@brief	Returns true, if the scheme is empty.
    ///	@return	true, if the URI is empty, false if not.
    bool isEmpty() const;

    ///	@brief	Parse the URI.
    ///	@return	true, if the URI is valid, false if not.
    bool parse();

    ///	@brief	Will return true, if the uri content is valid.
    ///	@return	true, if the uri itself is valid, false if not.
    bool isValid() const;

    ///	@brief	The URI representation will be cleared.
    void clear();

    /// @brief  Will return the extension
    /// @return the extension of the uri.
    String getExtension() const;

    /// @brief  Normalizes a path name.
    /// @param[inout] path          The path.
    /// @param[in]    sep           The path separator.
    /// @param[out]   normalized    The normalized path.
    /// @return false in case of an error.
    static bool normalizePath(const String &path, const c8 sep, String &normalized);

    // Needed operators
    bool operator == (const Uri &rhs) const;

private:
    String mURI;
    SchemeType mScheme;
    String mPath;
    String mAbsPath;
    String mResource;
};

} // Namespace OSRE::IO
