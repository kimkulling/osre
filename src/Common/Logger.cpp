/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/Logger.h>
#include <osre/Common/DateTime.h>

#include <cassert>

#ifdef OSRE_WINDOWS
//#  include "../Debugging/Win32/Win32DbgLogStream.h"
#endif // OSRE_WINDOWS

#include <iostream>
#include <sstream>
#include <iomanip>

namespace OSRE {
namespace Common {

static const String Line = \
    "====================================================================================================";

static void appendDomain( const String &domain, String &logMsg ) {
    if( !domain.empty() ) {
        logMsg += "( ";
        logMsg += domain;
        logMsg += " )";
    }
}

AbstractLogStream::AbstractLogStream() 
: m_IsActive( true ) {
    // empty
}

AbstractLogStream::~AbstractLogStream() {
    // empty
}

void AbstractLogStream::activate() {
    m_IsActive = true;
}

void AbstractLogStream::desactivate() {
    m_IsActive = false;
}

bool AbstractLogStream::isActive() const {
    return m_IsActive;
}

Logger *Logger::s_logger = nullptr;

Logger *Logger::create() {
    if ( nullptr == s_logger ) {
        s_logger = new Logger;
    }

    return s_logger;
}

Logger *Logger::getInstance() {
    if ( nullptr == s_logger ) {
        static_cast<void>( Logger::create() );
    }

    return s_logger;
}

void Logger::kill() {
    if ( s_logger ) {
        delete s_logger;
        s_logger = nullptr;
    }
}

void Logger::debug( const String &domain, const String &msg ) {
    String logMsg;

    logMsg += "Dbg:  ";
    logMsg += msg;

    appendDomain( domain, logMsg );

    print( logMsg );
}

void Logger::info( const String &domain, const String &msg ) {
    String logMsg;

    logMsg += "Info: ";
    logMsg += msg;

    appendDomain( domain, logMsg );

    print( logMsg );
}

void Logger::print( const String &msg, PrintMode mode ) {
    if ( msg.empty() ) {
        return;
    }

    if (msg.size() > 8) {
        if (msg[ 6 ] == '<' && msg[ 7 ] == '=') {
            m_intention -= 2;
        }
    }

    String logMsg;
    if (0 != m_intention) {
        for (ui32 i = 0; i < m_intention; i++) {
            logMsg += " ";
        }
    }

    logMsg += msg;
    if( WithDateTime == mode ) {
        logMsg += " ( ";
        logMsg += this->getDateTime( );
        logMsg += " )";
    }

    logMsg += " \n";
    for( ui32 i = 0; i<m_LogStreams.size(); ++i ) {
        AbstractLogStream *stream = m_LogStreams[ i ];
        if ( stream ) {
            stream->write( logMsg );
        }
    }

    if (msg.size() > 8) {
        if (msg[ 6 ] == '=' && msg[ 7 ] == '>') {
            m_intention += 2;
        }
    }

}

void Logger::warn( const String &domain, const String &msg ) {
    String logMsg;
    logMsg += "Warn: ";
    logMsg += msg;
    appendDomain( domain, logMsg );

    print( logMsg );
}

void Logger::error( const String &domain, const String &msg ) {
    String logMsg;
    logMsg += "Err:  ";
    logMsg += msg;
    appendDomain( domain, logMsg );

    print( logMsg );
}

void Logger::fatal( const String &domain, const String &msg ) {
    String logMsg;
    logMsg += "Fatal:";
    logMsg += msg;
    appendDomain( domain, logMsg );

    print( logMsg );
}

void Logger::registerLogStream( AbstractLogStream *pLogStream ) {
    if ( nullptr == pLogStream ) {
        return;
    }

    m_LogStreams.add( pLogStream );
}

void Logger::unregisterLogStream( AbstractLogStream *logStream ) {
    if ( nullptr != logStream ) {
        return;
    }

    for ( ui32 i=0; i<m_LogStreams.size(); ++i ) {
        if ( m_LogStreams[ i ] == logStream ) {
            delete m_LogStreams[ i ];
            m_LogStreams.remove( i );
        }
    }
}

Logger::Logger() 
: m_LogStreams()
, m_intention( 0 ) {
    m_LogStreams.add( new StdLogStream );

#ifdef OSRE_WINDOWS
//    m_LogStreams.add( new Debugging::Win32DbgLogStream );
#endif // OSRE_WINDOWS

    print( Line, WhithoutDateTime );
    print( "New OSRE run." );
    print( Line, WhithoutDateTime );
}

Logger::~Logger() {
    print( Line, WhithoutDateTime );
    print( "OSRE run ended." );
    print( Line, WhithoutDateTime );

    for ( ui32 i=0; i<m_LogStreams.size(); ++i ) {
        delete m_LogStreams[ i ];
    }
}

String Logger::getDateTime() {
    static const ui32 Space = 2;
    DateTime currentDateTime = DateTime::getCurrentUTCTime();
    std::stringstream stream;
    stream.fill( '0' );
    stream << std::setw ( Space ) << currentDateTime.getCurrentMonth() << "."
           << std::setw ( Space ) << currentDateTime.getCurrentDay() << "."
           << std::setw ( Space*2 ) << currentDateTime.getCurrentYear() << " "
           << std::setw ( Space ) << currentDateTime.getCurrentHour() << ":"
           << std::setw ( Space ) << currentDateTime.getCurrentMinute() << ":"
           << std::setw ( Space ) << currentDateTime.getCurrentSeconds()<< " ";
    
    return stream.str();
}

Logger::StdLogStream::StdLogStream() {
    // empty
}

Logger::StdLogStream::~StdLogStream() {
    // empty
}

void Logger::StdLogStream::write( const String &msg ) {
    std::cout << msg;
}

void debugPrint( const String &domain, const String &file, int line, const String &msg ) {
    String message;
    message += msg;
    message += " (";
    message += file;
    message += ", ";
    std::stringstream ss;
    ss << line;
    message += ss.str();
    message += ")";
    Logger::getInstance()->debug( domain, message );
}

void infoPrint( const String &domain, const String &file, int line, const String &msg ) {
    String message;
    message += msg;
    message += " (";
    message += file;
    message += ", ";
    std::stringstream ss;
    ss << line;
    message += ss.str();
    message += ")";
    Logger::getInstance()->info( domain, message );
}

void logPrint( const String &domain, const String &file, int line, const String &message ) {
    String msg;
    msg += file;
    msg += ", ";
    std::stringstream ss;
    ss << line;
    msg += ss.str();
    msg += " ";
    String tmp = domain + " : " + message;
    tmp += " (";
    tmp += msg;
    tmp += ")";
    Logger::getInstance()->print( tmp );
}

void warnPrint( const String &domain, const String &file, int line, const String &message ) {
    String msg;
    msg += message;
    msg += " (";
    msg += file;
    msg += ", ";
    std::stringstream ss;
    ss << line;
    msg += ss.str();
    msg += ")";
    Logger::getInstance()->warn( domain, msg );
}

void errorPrint( const String &domain, const String &file, int line, const String &message ) {
    String msg;
    msg += message;
    msg += " (";
    msg += file;
    msg += ", ";
    std::stringstream ss;
    ss << line;
    msg += ss.str();
    msg += ")";
    Logger::getInstance()->error( domain, msg );
}

void fatalPrint( const String &domain,  const String &file, int line, const String &message ) {
    String msg;
    msg += message;
    msg += " (";
    msg += file;
    msg += ", ";
    std::stringstream ss;
    ss << line;
    msg += ss.str();
    msg += ")";
    Logger::getInstance()->fatal( domain, msg );
}

} // Namespace Common
} // Namespace OSRE
