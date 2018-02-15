/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2018 OSRE ( Open Source Render Engine ) by Kim Kulling

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

#include <osre/Common/osre_common.h>
#include <osre/IO/Uri.h>

namespace OSRE {
namespace IO {

// Forward declarations
class Uri;

//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This class declares the interface for a stream. 
///
/// Streams can be used to do any kind of input-output operations like reading stuff from the hard 
/// disk or writing stuff into an archive.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT Stream {
public:
    typedef ui32 Position;      ///< The current position.
    typedef ui32 Offset;        ///< The offset from the current position.
    
    /// @brief  Enumerates the type of access.
    enum class AccessMode	{
        ReadAccess,             ///< Read-only access.
        WriteAccess,            ///< Write-access.
        ReadAccessBinary,       ///< Read-access in binary mode.
        WriteAccessBinary,      ///< Write-access in binary mode.
        ReadWriteAccess,        ///< Read/Write-access.
        AppendAccess            ///< Append-access, stuff will be attached and the file end.
    };

    /// @brief  Enumerates the requested file position
    enum class Origin	{
        Begin,      ///< Begin of the file.
        Current,    ///< Current position
        End         ///< End of the file.
    };

public:
    ///	@brief	The default class constructor.
    Stream() noexcept;

    ///	@brief	The class constructor with the uri and the access type.
    ///	@param	uri				[in] Describes the location of the object to access.
    ///	@param	mode        	[in] The requested access mode.
    Stream( const Uri &uri, AccessMode mode );

    ///	@brief	The class destructor, virtual.
    virtual ~Stream();

    ///	@brief	Set the uri manually.
    ///	@param	uri		[in] Describes the location of the object to access.
    virtual void setUri( const Uri &uRI);

    ///	@brief	Returns the assigned uri.
    ///	@return	A reference showing to the uri.
    virtual const Uri &getUri() const;

    /// @brief  Returns true, if stream can used to read.
    /// @return true, if read operation is supported.
    virtual bool canRead() const;

    /// @brief  Returns true, if stream can used to write.
    /// @return true, if write operation is supported.
    virtual bool canWrite() const;
    
    /// @brief  Returns true, if stream can used to seek the position.
    /// @return true, if seek operation is supported.
    virtual bool canSeek() const;
    
    /// @brief  Returns true, if stream can used to map to a memory location.
    /// @return true, if map operation is supported.
    virtual bool canBeMapped() const;

    /// @brief  Set the current request mode.
    /// @param  accessMode      [in] The new access mode.
    virtual void setAccessMode( AccessMode accessMode );

    /// @brief  Returns the current access mode.
    /// @return The current access mode.
    virtual AccessMode getAccessMode() const;
    
    /// @brief  Opens the stream.
    /// @return true, if successful.
    virtual bool open();

    /// @brief  Closes the stream.
    /// @return true, if successful.
    virtual bool close();
    
    /// @brief  Returns the file size.
    /// @return The file size.
    virtual ui32 getSize() const;
    
    /// @brief  Reads a given number of bytes from the stream.
    /// @param  buffer          [in] The buffer to read in.
    /// @param  size            [in] The number of bytes to read.
    /// @return The number of read bytes.
    virtual ui32 read( void *buffer, ui32 size );
    
    /// @brief  Writes a given number of bytes into the stream.
    /// @param  buffer          [out] The buffer to write.
    /// @param  size            [in] The number of bytes to write.
    /// @return The number of written bytes.
    virtual ui32 write( const void *buffer, ui32 size );
    
    /// @brief  Reads a given i32 value.
    /// @param  value       [out] The i32-value to read-
    /// @return The number of written bytes.
    virtual ui32 readI32( i32 &value );

    /// @brief  Writes a given i32 value into the stream.
    /// @param  value       [in] The i32-value to write.
    /// @return The number of written bytes.
    virtual ui32 writeI32( i32 value );

    /// @brief  Reads a given ui32 value.
    /// @param  value       [out] The ui32-value to read.
    /// @return The number of read bytes.
    virtual ui32 readUI32( ui32 &value );

    /// @brief  Writes a given ui32 value into the stream.
    /// @param  value       [in] The ui32-value to write.
    /// @return The number of written bytes.
    virtual ui32 writeUI32( ui32 value );

    /// @brief  Reads a given f32 value.
    /// @param  value       [out] The f32-value to read.
    /// @return The number of read bytes.
    virtual ui32 readF32( f32 &value );

    /// @brief  Writes a given f32 value into the stream.
    /// @param  value       [in] The f32-value to write.
    /// @return The number of written bytes.
    virtual ui32 writeF32( f32 value );

    /// @brief  Reads a given d32 value.
    /// @param  value       [out] The d32-value to read.
    /// @return The number of read bytes.
    virtual ui32 readD32( d32 &value );

    /// @brief  Writes a given d32 value into the stream.
    /// @param  value       [in] The d32-value to write.
    /// @return The number of written bytes.
    virtual ui32 writeD32( d32 value );

    /// @brief  Moves to the current position.
    /// @param  offset          [in] The offset.
    /// @param  origin          [in] The origin.
    /// @return The position.
    virtual Position seek( Offset offset, Origin origin );
    
    /// @brief  Returns the current position.
    /// @return The current position.
    virtual Position tell();
    
    ///	@brief	Returns true, if the stream is open.
    ///	@return true, if file is open.
    virtual bool isOpen() const;

public:
    Uri m_Uri;
    AccessMode m_AccessMode;
};

} // Namespace IO
} // Namespace OSRE
