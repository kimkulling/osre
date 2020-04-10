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

#include <osre/IO/IOCommon.h>
#include <osre/IO/Stream.h>
#include <cppcore/Container/TArray.h>

#include <map>

namespace OSRE {
namespace IO {

// Forward declarations
class Uri;
class Stream;

//--------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///	
///	@brief	Abstract interface for all file system implementations. Use this interface to build your 
///	own file system implementations.
//--------------------------------------------------------------------------------------------------
class OSRE_EXPORT AbstractFileSystem {
public:
    typedef std::map<String, Stream*> StreamMap;

public:
    ///	@brief	The class destructor, virtual.
    virtual ~AbstractFileSystem();

    ///	@brief	Opens a new file instance.
    ///	@param	filename    [in] Filename to open.
    ///	@param	mode        [in] Access mode.
    ///	@return	Opened file instance
    virtual Stream *open( const Uri &filename, Stream::AccessMode mode ) = 0;
    
    ///	@brief	Closes a given file instance
    ///	@param	pFile	File instance to close, must be opened before.
    virtual void close( Stream **pFile ) = 0;

    ///	@brief	Returns true, if file exists in file system.
    ///	@param	filename    [in] Filename to look for.
    ///	@return	true, if file exists.
    virtual bool fileExist( const Uri &filename) = 0;

    /// @brief  Search for a given file.
    /// @param  filename    [in] The filename to look for.
    /// @param  mode        [in] The requested access mode.
    /// @param  searchPaths [in] The search paths to look in.
    /// @return A pointer showing to the opened stream.
    virtual Stream *find(const Uri &filename, Stream::AccessMode mode, CPPCore::TArray<String> *searchPaths ) = 0;

    ///	@brief	Returns the schema description of the file system.
    ///	@return	The schema description.
    virtual const c8 *getSchema() const = 0;

    /// @brief  Will return the working directory.
    /// @return The working directory.
    virtual String getWorkingDirectory() = 0;

public:
    ///	@brief	Adds an ownership.
    void get();
    
    ///	@brief	Removes an ownership.
    void release();

    /// @brief  Will normalize a filename.
    /// @param  name    [inout] The filename to normalize.
    static void normalizeFilename( String &name ); 

protected:
    ///	@brief	The default class constructor
    AbstractFileSystem();

private:
    ui32 m_numberOfRefs;
};

inline
AbstractFileSystem::AbstractFileSystem() 
: m_numberOfRefs( 1 ) {
    // empty
}

inline
AbstractFileSystem::~AbstractFileSystem() {
    // empty
}

inline
void AbstractFileSystem::get() {
    ++m_numberOfRefs;
}

inline
void AbstractFileSystem::release() {
    if ( m_numberOfRefs ) {
        --m_numberOfRefs;
        if ( !m_numberOfRefs ) {
            delete this;
        }
    }
}

inline
void AbstractFileSystem::normalizeFilename( String &name ) {
    if ( name.empty() ) {
        return;
    }

    static const c8 bs = '\\';
    for ( String::iterator it = name.begin(); it != name.end(); ++it )	{
        if ( *it == bs ) {
            *it = '/';
        }
    }
}

} // Namespace IO
} // Namespace OSRE
