#pragma once

#include <osre/Common/AbstractService.h>
#include <osre/IO/Stream.h>

#include <map>

namespace OSRE {
namespace IO {

class AbstractFileSystem;
class Uri;

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::IO::IOService
///	@ingroup	Infrastructure
///
///	@brief	This class implements the IO-server, which offers access to all mounted file systems.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT IOService : public Common::AbstractService {
private:                                                                           
    static IOService * s_instance;                                                
public:
    static IOService * getInstance() { return s_instance; }
    static void   setInstance( IOService *pInstance ) { s_instance = pInstance; }
//    DECLARE_SINGLETON( IOService )

public:
    ///	@brief	The default class constructor.
    IOService();

    ///	@brief	The class destructor.
    ~IOService();

    ///	@brief	Will open the access to the IOServer.
    ///	@return	true, if the access was opened properly, false if not.
    bool onOpen();

    ///	@brief	Will close the access to the IOServer.
    ///	@return	true, if the access was opened properly, false if not.
    bool onClose();
    
    ///	@brief	The update callback, will be called once per frame.
    ///	@param	timeDiff	[in] The time passed since the last update was triggered.
    bool onUpdate( d32 timeDiff );

    /// @brief  A new file system will be added.
    ///	@param	name		[in] The name of the new file system.
    /// @param  file        [in] The Uri of the new file system.
    /// @return A pointer showing to the file system will be returned.
    AbstractFileSystem *addFileSystem( const String &name, const Uri &file );

    ///	@brief	A new file system will be mounted using the given schema.
    ///	@param	name		[in] The name of the new file system.
    ///	@param	pFileSystem	[in] A pointer showing to the new file system instance.
    void mountFileSystem( const String &name, AbstractFileSystem *pFileSystem );

    ///	@brief	A mounted file system will be unmounted.
    ///	@param	name		[in] The name of the mounted file system.
    ///	@param	pFileSystem	[in] A pointer showing to the mounted file system instance.
    void umountFileSystem( const String &name, AbstractFileSystem *pFileSystem );

    /// @brief  A new stream will be opened, the corresponding file system will be used.
    /// @param  file        [in] The file name as an Uri.
    /// @param  mode        [in] The access mode.
    /// @return A pointer showing to the stream or nullptr in case of an error.
    Stream *openStream( const Uri &file, Stream::AccessMode mode );
    
    /// @brief  Will close a opened stream.
    /// @param  ppStream    [in] The pointer to the stream pointer, will be nullptr afterwards.
    void closeStream( Stream **ppStream );

    ///	@brief	Returns the assigned file system to a schema.
    ///	@param	schema		[in] The schema description of the mounted file system.
    ///	@return	A pointer showing to the file system or NULL, if no file system is mounted 
    ///	to the given scheme.
    AbstractFileSystem *getFileSystem(const String &schema ) const;
    
    /// @brief  A wrapper class to be able to look if a file exists or not.
    /// @param  file        [in] The Uri with the file location.
    /// @return true, if the file exists.
    bool fileExists( const Uri &file ) const;
    
    /// @brief  Will create a new instance.
    /// @return The new created instance.
    static IOService *create();

    /// @brief  Normalizes a path name.
    /// @param  path        [in-out] The path.
    /// @param  sep         [in] The path separator.
    /// @param  normalized  [out] The normalized path.
    /// @return false in case of an error.
    static bool normalizePath( const String &path, const c8 sep, String &normalized );

private:
    typedef std::map<String, AbstractFileSystem*> MountedMap;
    MountedMap m_mountedMap;
};

//-------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
