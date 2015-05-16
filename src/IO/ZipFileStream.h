#pragma once

#include <osre/IO/Stream.h>

#include "unzip.h"

namespace OSRE {
namespace IO {

//--------------------------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::IO::ZipFileStream
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
	~ZipFileStream();
	///	Read operations are supported.
	bool canRead() const;
	///	Reads data from a file in a zip archive.
	ui32 read( void *pBuffer, size_t size );
	///	Returns the file size for a file stored in a zip archive.
	ui32 getSize();
	///	Returns true, if the file is currently open.
	bool isOpen() const;

private:
	void sortFiles();

private:
	unzFile m_zipFile;
	bool m_bDirty;
};

//--------------------------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
