#pragma once

#include <osre/IO/AbstractFileSystem.h>

namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::IO::BaseFileSystem
///	@ingroup	Infrastructure
///
///	@brief  This class implements a file system stored on a hard dick.
//-------------------------------------------------------------------------------------------------
class LocaleFileSystem : public AbstractFileSystem {
public:
	///	The default class constructor.
	LocaleFileSystem();
	///	The class destructor, virtual.
	virtual ~LocaleFileSystem();
	///	Opens a file on the local file system and returns the stream pointing onto the file.
	virtual Stream *open( const Uri &rFile, Stream::AccessMode mode );
	///	The stream will be closed.
	virtual void close( Stream **pFile );
	///	Returns true, if the file exists.
	virtual bool fileExist( const Uri &rFile );
	///	Looks for a file and returns the stream.	
	virtual Stream *find(const Uri &rFile, Stream::AccessMode mode, CPPCore::TArray<String> *pSearchPaths );
	///	returns the schema description of the file system.
	virtual const String &getSchema() const;
	///	Returns the working directory.
	virtual String getWorkingDirectory();

private:
	StreamMap m_FileMap;
};

//-------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
