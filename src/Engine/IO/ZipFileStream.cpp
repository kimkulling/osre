/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "ZipFileStream.h"
#include <cassert>

#include "unzip.h"

#include <algorithm>
#include <iostream>

namespace OSRE {
namespace IO {

ZipFileStream::ZipFileStream( const Uri &uri, unzFile zipFile ) 
: Stream( uri , AccessMode::ReadAccess )
, m_zipFile( zipFile )
, m_bDirty( true ) {
	assert( NULL != zipFile );
}

ZipFileStream::~ZipFileStream() {
	if ( NULL != m_zipFile ) {
		unzClose( m_zipFile );
		m_zipFile = NULL;
	}
}

bool ZipFileStream::canRead() const {
	return true;
}

ui32 ZipFileStream::read( void *buffer, ui32 size ) {
	assert( NULL != buffer );
	assert( NULL != m_zipFile );
	
	ui32 filesize = 0;
    if ( 0 == size ) {
        return filesize;
    }

	if ( NULL != m_zipFile ) {
		// get the filename
		const String &abspath = getUri().getAbsPath();

		// search file and place file pointer there
		if ( unzLocateFile( m_zipFile, abspath.c_str(), 0 ) == UNZ_OK ) {
			// get file size, etc.
			unz_file_info fileInfo;
			unzGetCurrentFileInfo( m_zipFile, &fileInfo, 0, 0, 0, 0, 0, 0 );
			assert( size <= fileInfo.uncompressed_size );
			
			// The file has EXACTLY the size of uncompressed_size. In C
			// you need to mark the last character with '\0', so add 
			// another character
			unzOpenCurrentFile( m_zipFile );
			i32 bytesRead = unzReadCurrentFile( m_zipFile, buffer, fileInfo.uncompressed_size);
			if ( bytesRead < 0 || bytesRead != (i32) fileInfo.uncompressed_size ) {
				return filesize;
			}

			filesize = fileInfo.uncompressed_size;
			unzCloseCurrentFile( m_zipFile );
		}
	}

	return filesize;
}

ui32 ZipFileStream::getSize() const {
	// get the filename
	const String &abspath = getUri().getAbsPath();

	// look for the file and return its size
	if ( unzLocateFile( m_zipFile, abspath.c_str(), 0) == UNZ_OK )  {
		unz_file_info fileInfo;
		unzGetCurrentFileInfo(m_zipFile, &fileInfo, 0, 0, 0, 0, 0, 0);
		return fileInfo.uncompressed_size;
	}

	return 0;
}

bool ZipFileStream::isOpen() const {
	return ( NULL != m_zipFile );
}

} // Namespace IO
} // Namespace OSRE
