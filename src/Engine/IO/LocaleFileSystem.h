/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2021 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/IO/AbstractFileSystem.h>

namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::IO::BaseFileSystem
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
	virtual const c8 *getSchema() const;
	///	Returns the working directory.
	virtual String getWorkingDirectory();

private:
	StreamMap m_FileMap;
};

} // Namespace IO
} // Namespace OSRE
