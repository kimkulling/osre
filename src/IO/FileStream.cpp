/* ZFX Community Engine 2  (ZFXCE2)
---------------------------------------------------------------------------------------------------
Copyright (c) 2011-2015, ZFXCE2 Development Team
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
-------------------------------------------------------------------------------------------------*/
#include "FileStream.h"

#include <cassert>
#include <sys/types.h> 
#include <sys/stat.h> 

namespace OSRE {
namespace IO {

//-------------------------------------------------------------------------------------------------
FileStream::FileStream() 
: Stream()
, m_pFile( nullptr ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
FileStream::FileStream( const Uri &uri, AccessMode requestedAccess ) 
: Stream( uri, requestedAccess )
, m_pFile( nullptr ) {
    // empty
}

//-------------------------------------------------------------------------------------------------
FileStream::~FileStream() {
    if ( isOpen() )	{
        this->close();
    }
}

//-------------------------------------------------------------------------------------------------
bool FileStream::canRead() const {
    return true;
}

//-------------------------------------------------------------------------------------------------
bool FileStream::canWrite() const {
    return true;
}

//-------------------------------------------------------------------------------------------------
bool FileStream::canSeek() const {
    return true;
}

//-------------------------------------------------------------------------------------------------
bool FileStream::open() {
    if ( isOpen() ) {
        return false;
    }

    assert( nullptr == m_pFile );

    const String &abspath = m_Uri.getAbsPath();
    String modestr;
    AccessMode mode = getAccessMode();
    if ( ReadAccessBinary == mode ) {
        modestr = "rb";
    } else if ( WriteAccessBinary == mode ) {
        modestr = "wb";
    } else if ( ReadAccess == mode ) {
        modestr = "r";
    } else if ( WriteAccess == mode ) {
        modestr = "w";
    } else if ( ReadWriteAccess == mode ) {
        modestr = "r+";
    } else if ( AppendAccess == mode ) {
        modestr = "ra";
    }
    assert( !modestr.empty() );

#if defined(CE_WINDOWS) && !defined(__MINGW32__) && !defined (__MINGW64__)
    errno_t err;
    err = ::fopen_s( &m_pFile, abspath.c_str(), modestr.c_str() );
    assert( 0 == err );
#else
    m_pFile = ::fopen( abspath.c_str(), modestr.c_str() );
#endif 

    return ( nullptr != m_pFile );
}

//-------------------------------------------------------------------------------------------------
bool FileStream::close() {
    if ( !isOpen() ) {
        return false;
    }

    assert( nullptr != m_pFile );
    if ( m_pFile ) {
        ::fclose( m_pFile );
        m_pFile = nullptr;
        return true;
    } else {
        return false;
    }
}

//-------------------------------------------------------------------------------------------------
ui32 FileStream::getSize() {	
    assert( !m_Uri.getAbsPath().empty() );

    const String &abspath = m_Uri.getAbsPath();

#ifdef CE_WINDOWS
    // Get the windows specific file-size
    struct __stat64 fileStat; 
    int err = _stat64(  abspath.c_str(), &fileStat ); 
    if ( 0 != err ) {
        return 0;
    } else {
        return ( static_cast<ui32>( fileStat.st_size ) ); 
    }

#else
    // For unix
    struct stat fileStat; 
    int err = stat( abspath.c_str(), &fileStat ); 
    if ( 0 != err ) 
        return 0; 
    return  static_cast<ui32>( fileStat.st_size ); 
#endif
}

//-------------------------------------------------------------------------------------------------
ui32 FileStream::read( void *pBuffer, ui32 size ) {
    if ( !pBuffer  || 0 == size ) {
        return 0;
    }

    if ( !isOpen() ) {
        return 0;
    }
    
    assert( nullptr != m_pFile );
    if ( m_pFile ) {
        return ( static_cast<ui32>( ::fread( pBuffer, sizeof( uc8 ), size , m_pFile ) ) );
    } else {
        return 0;
    }
}

//-------------------------------------------------------------------------------------------------
ui32 FileStream::write( const void *pBuffer, ui32 size ) {
    assert( nullptr != pBuffer );
    if ( !isOpen() || 0 == size || !pBuffer ) {
        return 0;
    }

    assert( nullptr != m_pFile );
    if ( m_pFile ) {
        return ( static_cast<ui32>( ::fwrite( pBuffer, sizeof( c8 ), size, m_pFile ) ) );
    } else {
        return 0;
    }
}

//-------------------------------------------------------------------------------------------------
FileStream::Position FileStream::seek( Offset offset, Origin origin ) {
    assert( nullptr != m_pFile );

    i32 originValue = 0;
    if ( origin == Current ) {
        originValue = SEEK_CUR;
    } else {
        originValue = SEEK_SET;
    }
    if ( m_pFile ) {
        return ::fseek( m_pFile, offset, originValue );
    } else {
        return 0;
    }
}

//-------------------------------------------------------------------------------------------------
FileStream::Position FileStream::tell() {
    assert( nullptr != m_pFile );
    if ( m_pFile ) {
        return ( ::ftell( m_pFile ) );
    } else {
        return 0;
    }
}

//-------------------------------------------------------------------------------------------------
bool FileStream::isOpen() const {
    return ( m_pFile != NULL );
}

//-------------------------------------------------------------------------------------------------

} // Namespace IO
} // Namespace OSRE
