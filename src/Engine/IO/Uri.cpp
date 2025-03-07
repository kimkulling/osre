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
#include "IO/Uri.h"

#include <assimp/ParsingUtils.h>

#include <algorithm>

namespace OSRE::IO {

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

Uri::Uri(const String &uri) :
		mURI(uri), mScheme( "" ), mPath( "" ), mAbsPath( "" ), mResource( "" ) {
    String normailizedStr;
    if (Uri::normalizePath( uri, '\\', normailizedStr )) {
        if (isWindowsRootFolder( normailizedStr )) {
            mURI = converRootFolder2Uri( normailizedStr );
        }
        static_cast<void>(parse());
    }
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
	if ( uri == mURI ) {
		return;
	}

	clear();
	mURI = uri;
	static_cast<void>(parse());
}

const String &Uri::getUri() const {
	return mURI;
}

const String &Uri::getScheme() const {
	return mScheme;
}

void Uri::setScheme( const String &scheme ) {
    if ( mScheme != scheme ) {
        mScheme = scheme;
        mURI = constructFromComps( mScheme, mPath, mResource );
    }
}

const String &Uri::getPath() const {
	return mPath;
}

void Uri::setPath( const String &path ) {
    if ( mPath != path ) {
        mPath = path;
        mAbsPath = path;
        mURI = constructFromComps( mScheme, mPath, mResource );
    }
}

const String &Uri::getAbsPath() const {
	return mAbsPath;
}

const String &Uri::getResource() const {
	return mResource;
}

void Uri::setResource( const String &res ) {
    if ( mResource != res ) {
        mResource = res;
        mURI = constructFromComps( mScheme, mPath, mResource );
    }
}

bool Uri::isEmpty() const {
	return mURI.empty();
}

bool Uri::parse() {
	if ( mURI.empty() ) {
		clear();
		return false;
	}

	// validate the URI syntax
	String::size_type pos0 = mURI.find( "://" );
	if ( String::npos == pos0 ) {
		clear();
		return false;
	}


	// extract the schema type
	mScheme = mURI.substr( 0, pos0 );
	String::size_type pos1 = mURI.rfind( "/" );
	if ( String::npos == pos1 ) {
		return false;
	}

	// get the length
	const size_t resLen = mURI.size() - pos1;
	const size_t len    = mURI.size() - pos0 - resLen - 2;
	
	// get the components
	mPath     = mURI.substr( pos0 + 3, len );
	mResource = mURI.substr( pos1 + 1, mURI.size() - pos1 );
	mAbsPath  = mPath + mResource;

	return true;
}

bool Uri::isValid() const {
	return !mAbsPath.empty();
}

void Uri::clear() {
	mScheme.clear();
	mPath.clear();
	mAbsPath.clear();
	mResource.clear();
    mURI.clear();
}

String Uri::getExtension() const {
    String ext;
	if (!isValid() || isEmpty()) {
        return ext;
	}

	String ::size_type pos = mResource.rfind('.');
    if (pos == String::npos) {
        return ext;
	}

	ext = mResource.substr(pos + 1, mResource.size() - pos - 1);
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

bool Uri::operator==(const Uri& rhs) const {
    return mURI == rhs.mURI;
}

} // Namespace OSRE::IO
