/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2016 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "FileStream.h"

#include <osre/Debugging/osre_debugging.h>
#include <sys/types.h> 
#include <sys/stat.h> 

namespace OSRE {
namespace IO {

FileStream::FileStream() 
: Stream()
, m_pFile( nullptr ) {
    // empty
}

FileStream::FileStream( const Uri &uri, AccessMode requestedAccess ) 
: Stream( uri, requestedAccess )
, m_pFile( nullptr ) {
    // empty
}

FileStream::~FileStream() {
    if ( isOpen() )	{
        this->close();
    }
}

bool 
FileStream::canRead() const {
    return true;
}

bool FileStream::canWrite() const {
    return true;
}

bool FileStream::canSeek() const {
    return true;
}

bool FileStream::open() {
    if ( isOpen() ) {
        return false;
    }

    OSRE_ASSERT( nullptr == m_pFile );
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
    OSRE_ASSERT( !modestr.empty() );

#if defined(OSRE_WINDOWS) && !defined(__MINGW32__) && !defined (__MINGW64__)
    errno_t err;
    err = ::fopen_s( &m_pFile, abspath.c_str(), modestr.c_str() );
    OSRE_ASSERT( 0 == err );
#else
    m_pFile = ::fopen( abspath.c_str(), modestr.c_str() );
#endif 

    return ( nullptr != m_pFile );
}

bool FileStream::close() {
    if ( !isOpen() ) {
        return false;
    }

    OSRE_ASSERT( nullptr != m_pFile );
    if ( m_pFile ) {
        ::fclose( m_pFile );
        m_pFile = nullptr;
        return true;
    } else {
        return false;
    }
}

ui32 FileStream::getSize() const {	
    OSRE_ASSERT( !m_Uri.getAbsPath().empty() );

    const String &abspath( m_Uri.getAbsPath() );
#ifdef OSRE_WINDOWS
    // Get the windows specific file-size
    struct __stat64 fileStat; 
    int err = _stat64(  abspath.c_str(), &fileStat ); 
    if ( 0 != err ) {
        return 0;
    }

    return ( static_cast<ui32>( fileStat.st_size ) ); 
#else
    // For unix
    struct stat fileStat; 
    int err = stat( abspath.c_str(), &fileStat ); 
    if ( 0 != err ) {
        return 0;
    }
    return  static_cast<ui32>( fileStat.st_size ); 
#endif
}

ui32 FileStream::read( void *pBuffer, ui32 size ) {
    if ( !pBuffer  || 0 == size ) {
        return 0;
    }

    if ( !isOpen() ) {
        return 0;
    }
    
    OSRE_ASSERT( nullptr != m_pFile );
    if ( m_pFile ) {
        return ( static_cast<ui32>( ::fread( pBuffer, sizeof( uc8 ), size , m_pFile ) ) );
    } 
    return 0;
}

ui32 FileStream::write( const void *pBuffer, ui32 size ) {
    OSRE_ASSERT( nullptr != pBuffer );
    if ( !isOpen() || 0 == size || !pBuffer ) {
        return 0;
    }

    OSRE_ASSERT( nullptr != m_pFile );
    if ( m_pFile ) {
        return ( static_cast<ui32>( ::fwrite( pBuffer, sizeof( c8 ), size, m_pFile ) ) );
    } 
      
    return 0;
}

ui32 FileStream::readI32( i32 &value ) {
    OSRE_ASSERT( nullptr != m_pFile );
    if ( m_pFile ) {
        return ( static_cast< ui32 >( ::fread( &value, sizeof( i32 ), 1, m_pFile ) ) );
    }

    return 0;
}

ui32 FileStream::writeI32( i32 value ) {
    OSRE_ASSERT( nullptr != m_pFile );
    if ( m_pFile ) {
        return ( static_cast< ui32 >( ::fwrite( &value, sizeof( i32 ), 1, m_pFile ) ) );
    }
    
    return 0;
}

ui32 FileStream::readUI32( ui32 &value ) {
    OSRE_ASSERT( nullptr != m_pFile );
    if ( m_pFile ) {
        return ( static_cast< ui32 >( ::fread( &value, sizeof( ui32 ), 1, m_pFile ) ) );
    }

    return 0;
}

ui32 FileStream::writeUI32( ui32 value ) {
    OSRE_ASSERT( nullptr != m_pFile );
    if ( m_pFile ) {
        return ( static_cast< ui32 >( ::fwrite( &value, sizeof( ui32 ), 1, m_pFile ) ) );
    }

    return 0;
}

FileStream::Position FileStream::seek( Offset offset, Origin origin ) {
    OSRE_ASSERT( nullptr != m_pFile );

    i32 originValue( 0 );
    if ( origin == Current ) {
        originValue = SEEK_CUR;
    } else {
        originValue = SEEK_SET;
    }
    if ( m_pFile ) {
        return ::fseek( m_pFile, offset, originValue );
    } 
      
    return 0;
}

FileStream::Position FileStream::tell() {
    OSRE_ASSERT( nullptr != m_pFile );
    if ( m_pFile ) {
        return ( ::ftell( m_pFile ) );
    }
     
    return 0;
}

bool FileStream::isOpen() const {
    return ( m_pFile != nullptr );
}

} // Namespace IO
} // Namespace OSRE
