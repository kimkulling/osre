/*
ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011, ZFXCE2 Development Team
All rights reserved.

Redistribution and use of this software in source and binary forms, 
with or without modification, are permitted provided that the 
following conditions are met:

* Redistributions of source code must retain the above copyright notice, this list of conditions 
  and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice, this list of conditions
  and the following disclaimer in the documentation and/or other materials provided with the 
  distribution.

* Neither the name of the ZFXCE2 team, nor the names of its contributors may be used to endorse or 
  promote products derived from this software without specific prior written permission of the 
  ZFXCE2 Development Team.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR 
IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR 
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL 
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER 
IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
---------------------------------------------------------------------------------------------------
*/
#include "ZipFileStream.h"
#include <cassert>

#include "unzip.h"

#include <algorithm>
#include <iostream>

namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
ZipFileStream::ZipFileStream( const Uri &rURI, unzFile zipFile ) :
	Stream( rURI , ReadAccess ),
	m_zipFile( zipFile ),
	m_bDirty( true )
{
	assert( NULL != zipFile );
}

//-------------------------------------------------------------------------------------------------
ZipFileStream::~ZipFileStream() {
	if ( NULL != m_zipFile ) {
		unzClose( m_zipFile );
		m_zipFile = NULL;
	}
}

//-------------------------------------------------------------------------------------------------
bool ZipFileStream::canRead() const {
	return true;
}

//-------------------------------------------------------------------------------------------------
ui32 ZipFileStream::read( void *pBuffer, size_t size ) {
	assert( NULL != pBuffer );
	assert( NULL != m_zipFile );
	
	ui32 filesize = 0;
	if ( 0 == size)
		return filesize;

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
			i32 bytesRead = unzReadCurrentFile( m_zipFile, pBuffer, fileInfo.uncompressed_size);
			if ( bytesRead < 0 || bytesRead != (i32) fileInfo.uncompressed_size ) {
				return filesize;
			}

			filesize = fileInfo.uncompressed_size;
			unzCloseCurrentFile( m_zipFile );
		}
	}

	return filesize;
}

//-------------------------------------------------------------------------------------------------
ui32 ZipFileStream::getSize() {
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

//-------------------------------------------------------------------------------------------------
bool ZipFileStream::isOpen() const {
	return ( NULL != m_zipFile );
}

//-------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
