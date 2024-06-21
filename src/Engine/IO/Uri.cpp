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
#include "IO/Uri.h"

#include <assimp/ParsingUtils.h>

#include <algorithm>

namespace OSRE {
namespace IO {

static bool isWindowsRootFolder(const String &filename) {
    if (filename.empty()) {
        return false;
    }

    if (Assimp::IsUpper(filename[0])) {
        if (filename[1] == ':') {
            return true;
        }
    }

    return false;
}

static String converRootFolder2Uri(const String &filename) {
    String uri;
    uri += "file://";
    uri += filename;
    
    return uri;
}

Uri::Uri() : m_URI( "" ), m_Scheme( "" ), m_Path( "" ), m_AbsPath( "" ), m_Resource( "" ) {
	// empty
}

Uri::Uri( const String &uri ) :
		m_URI(uri), m_Scheme( "" ), m_Path( "" ), m_AbsPath( "" ), m_Resource( "" ) {
    String normailizedStr;
    if (Uri::normalizePath( uri, '\\', normailizedStr )) {
        if (isWindowsRootFolder( normailizedStr )) {
            m_URI = converRootFolder2Uri( normailizedStr );
        }
        static_cast<void>(parse());
    }
}

Uri::Uri( const Uri &rhs ) :
		m_URI( rhs.m_URI ),
		m_Scheme( rhs.m_Scheme ),
		m_Path( rhs.m_Path ),
		m_AbsPath( rhs.m_AbsPath ),
		m_Resource( rhs.m_Resource ) {
	// empty
}

Uri::~Uri() {
	// empty
}

String Uri::constructFromComps( const String &scheme, const String &path, const String &resName ) {
    String uriText = scheme + "://" + path;
    String::size_type pos( uriText.rfind( resName ) );
    if ( String::npos == pos ) {
        uriText += resName;
    }
    return uriText;
}

void Uri::setUri(  const String &uri ) {
	if ( uri == m_URI ) {
		return;
	}

	clear();
	m_URI = uri;
	static_cast<void>( parse() );
}

const String &Uri::getUri() const {
	return m_URI;
}

const String &Uri::getScheme() const {
	return m_Scheme;
}

void Uri::setScheme( const String &scheme ) {
    if ( m_Scheme != scheme ) {
        m_Scheme = scheme;
        m_URI = constructFromComps( m_Scheme, m_Path, m_Resource );
    }
}

const String &Uri::getPath() const {
	return m_Path;
}

void Uri::setPath( const String &path ) {
    if ( m_Path != path ) {
        m_Path = path;
        m_AbsPath = path;
        m_URI = constructFromComps( m_Scheme, m_Path, m_Resource );
    }
}

const String &Uri::getAbsPath() const {
	return m_AbsPath;
}

const String &Uri::getResource() const {
	return m_Resource;
}

void Uri::setResource( const String &res ) {
    if ( m_Resource != res ) {
        m_Resource = res;
        m_URI = constructFromComps( m_Scheme, m_Path, m_Resource );
    }
}

bool Uri::isEmpty() const {
	return m_URI.empty();
}

bool Uri::parse() {
	if ( m_URI.empty() ) {
		clear();
		return false;
	}

	// validate the URI syntax
	String::size_type pos0 = m_URI.find( "://" );
	if ( String::npos == pos0 ) {
		clear();
		return false;
	}


	// extract the schema type
	m_Scheme = m_URI.substr( 0, pos0 );
	String::size_type pos1 = m_URI.rfind( "/" );
	if ( String::npos == pos1 ) {
		return false;
	}

	// get the length
	const size_t resLen = m_URI.size() - pos1;
	const size_t len    = m_URI.size() - pos0 - resLen - 2;
	
	// get the components
	m_Path     = m_URI.substr( pos0 + 3, len );
	m_Resource = m_URI.substr( pos1 + 1, m_URI.size() - pos1 );
	m_AbsPath  = m_Path + m_Resource;

	return true;
}

bool Uri::isValid() const {
	return !m_AbsPath.empty();
}

void Uri::clear() {
	m_Scheme.clear();
	m_Path.clear();
	m_AbsPath.clear();
	m_Resource.clear();
    m_URI.clear();
}

String Uri::getExtension() const {
    String ext;
	if (!isValid() || isEmpty()) {
        return ext;
	}

	String ::size_type pos = m_Resource.rfind('.');
    if (pos == String::npos) {
        return ext;
	}

	ext = m_Resource.substr(pos + 1, m_Resource.size() - pos - 1);
    return ext;
}

bool Uri::normalizePath(const String &path, const c8 sep, String &normalized) {
    normalized = "";
    if (path.empty()) {
        return true;
    }
    normalized = path;
    std::replace(normalized.begin(), normalized.end(), sep, '/');

    return true;
}

Uri &Uri::operator = ( const Uri &rhs ) {
	if ( !( *this == rhs ) ) {
		m_URI      = rhs.m_URI;
		m_Scheme   = rhs.m_Scheme;
		m_Path     = rhs.m_Path;
		m_AbsPath  = rhs.m_AbsPath;
		m_Resource = rhs.m_Resource;
	}

	return *this;
}

bool Uri::operator == ( const Uri &rhs ) const {
	return (m_URI      == rhs.m_URI && 
			m_Scheme   == rhs.m_Scheme && 
			m_Path     == rhs.m_Path && 
			m_AbsPath  == rhs.m_AbsPath && 
			m_Resource == rhs.m_Resource );
}

bool Uri::operator != ( const Uri &rhs ) const {
    return !( *this == rhs );
}

} // Namespace IO
} // Namespace OSRE
