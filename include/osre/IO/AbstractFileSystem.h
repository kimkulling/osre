#pragma once

#include <osre/IO/Stream.h>
#include <cppcore/Container/TArray.h>

#include <map>

namespace OSRE {
namespace IO {

class Uri;

//--------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::IO::AbstractFileSystem
///	@ingroup	Infrastructure
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
    ///	@param	rFilename	Filename to open.
    ///	@param	rFileMode	Access mode.
    ///	@return	Opened file instance
    virtual Stream *open( const Uri &rFile, Stream::AccessMode mode ) = 0;
    
    ///	@brief	Close a given file instance
    ///	@param	pFile	File instance to close, must be opened before.
    virtual void close( Stream **pFile ) = 0;

    ///	@brief	Returns true, if file exists in file system.
    ///	@param	rFileName	Filename to look for.
    ///	@return	true, if file exists.
    virtual bool fileExist( const Uri &rFile ) = 0;

    /// @brief  Search for a given file.
    virtual Stream *find(const Uri &rFile, Stream::AccessMode mode, CPPCore::TArray<String> *pSearchPaths ) = 0;

    ///	@brief	Returns the schema description of the file system.
    ///	@return	The schema description.
    virtual const String &getSchema() const = 0;

    /// @brief  Will return the working directory.
    /// @return The working directory.
    virtual String getWorkingDirectory() = 0;

public:
    ///	@brief	Adds an ownership.
    void get();
    
    ///	@brief	Removes an ownership.
    void release();

    static void normalizeFilename( String &rName ); 

protected:
    ///	@brief	The default class constructor
    AbstractFileSystem();

private:
    ui32 m_NumberOfRefs;
};

//--------------------------------------------------------------------------------------------------------------------
inline
AbstractFileSystem::AbstractFileSystem() 
: m_NumberOfRefs( 1 ) {
    // empty
}

//--------------------------------------------------------------------------------------------------------------------
inline
AbstractFileSystem::~AbstractFileSystem() {
    // empty
}

//--------------------------------------------------------------------------------------------------------------------
inline
void AbstractFileSystem::get() {
    ++m_NumberOfRefs;
}

//--------------------------------------------------------------------------------------------------------------------
inline
void AbstractFileSystem::release() {
    if ( m_NumberOfRefs ) {
        --m_NumberOfRefs;
        if ( !m_NumberOfRefs ) {
            delete this;
        }
    }
}

//--------------------------------------------------------------------------------------------------------------------
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

//--------------------------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
