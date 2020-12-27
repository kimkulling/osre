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
#pragma once

#include <osre/IO/Stream.h>

#include "unzip.h"

namespace OSRE {
namespace IO {

//--------------------------------------------------------------------------------------------------------------------
///	@class		::OSRE::IO::ZipFileStream
///	@ingroup	Infrastructure
///
///	@brief	File instance for files stored in a zip archive. 
///
/// If you requests access to data in a zip archive the zip file-system will return you a pointer to a zip file. 
//--------------------------------------------------------------------------------------------------------------------
class ZipFileStream : public Stream {
public:
	///	The class constructor.
	ZipFileStream( const Uri &rURI, unzFile zipFile );
	///	The class destructor.
	~ZipFileStream() override;
	///	Read operations are supported.
	bool canRead() const override;
	///	Reads data from a file in a zip archive.
	ui32 read( void *pBuffer, ui32 size ) override;
	///	Returns the file size for a file stored in a zip archive.
	ui32 getSize() const override;
	///	Returns true, if the file is currently open.
	bool isOpen() const override;

private:
	void sortFiles();

private:
	unzFile m_zipFile;
	bool m_bDirty;
};

//--------------------------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
