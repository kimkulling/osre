/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2024 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/AbstractService.h>
#include <osre/IO/Stream.h>

#include <map>

namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief	This class implements the IO-server, which offers access to all mounted file systems.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT IOService : public Common::AbstractService {
public:
    DECLARE_SINGLETON( IOService )

public:
    ///	@brief	The default class constructor.
    IOService();

    ///	@brief	The class destructor.
    ~IOService();

    ///	@brief	Will open the access to the IOServer.
    ///	@return	true, if the access was opened properly, false if not.
    bool onOpen() override;

    ///	@brief	Will close the access to the IOServer.
    ///	@return	true, if the access was opened properly, false if not.
    bool onClose() override;
    
    ///	@brief	The update callback, will be called once per frame.
    bool onUpdate() override;

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
    /// @param  stream      [in] The pointer to the stream pointer, will be nullptr afterwards.
    void closeStream( Stream **stream );

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

private:
    using MountedMap = std::map<String, AbstractFileSystem*> ;
    MountedMap m_mountedMap;
};

} // Namespace IO
} // Namespace OSRE
