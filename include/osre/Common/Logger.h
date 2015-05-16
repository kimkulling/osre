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
#pragma once

#include <osre/Common/Types.h>
#include <cppcore/Container/TArray.h>
#include <sstream>

namespace OSRE {
namespace Common {

//-------------------------------------------------------------------------------------------------
///	@class		::OSRE::Common::AbstractLogStream
///
///	@brief	This pure abstract interface class declares the protocol for log-stream implementation.
///	You can use log streams to pipes logs to you preferred output device like a simple text-file 
///	or a log window in a GUI application. Implement the write message and attach a valid instance 
///	to the logger itself to use your own implementation.
//-------------------------------------------------------------------------------------------------
class OSRE_DLL_EXPORT AbstractLogStream {
public:
    ///	@brief	The class destructor, virtual.
    virtual ~AbstractLogStream();

    ///	@brief	Will write a message into the attached log stream.
    ///	@param	rMessage	The message to log.
    virtual void write( const String &rMessage ) = 0;

    ///	@brief	The stream will be activated.
    virtual void activate();

    ///	@brief	The stream will be deactivated.
    virtual void desactivate();

    ///	@brief	Returns true, if the stream is active.
    ///	@return	true, if the stream is active, else false will be returned.
    virtual bool isActive() const;

protected:
    ///	@brief	The default class constructor.
    AbstractLogStream();

private:
    bool m_IsActive;
};

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Common::Logger
///	@ingroup	Infrastructure
///
///	@brief	This class implements a simple logger. 
///
///	The logger is implemented as a singleton. You can attach several log streams to it, which can
///	be used to log the messages to several output channels like a window or a log file or something 
///	else.
///	The granularity of the logged messages can be controlled by the severity of the logger. The 
///	supported modes are normal ( no debug and info messages ), verbose ( all messages will be
///	logged ) and debug ( the debug messages will be logged as well, be careful with this option ).
//-------------------------------------------------------------------------------------------------
class OSRE_DLL_EXPORT Logger {
public:
    ///	@enum	Severity
    ///	@brief	Describes the 
    enum Severity {
        Normal,		///< Only warnings and errors will be logged.
        Verbose,	///< Normal messages will be logged as well.
        Debug		///< All debug messages will be logged as well.
    };

    ///	@enum	PrintMode
    ///	@brief	Describes the mode for prints into the active log stream.
    enum PrintMode {
        WithDateTime,		///< A dateTime string will put be in front of the entry.
        WhithoutDateTime	///< No DateTime will be there.
    };

public:
    ///	@brief	Creates the unique logger instance and returns a pointer showing to it.
    ///	@return	The singleton pointer of the logger.
    static Logger *create();

    ///	@brief	returns the singleton instance pointer of the logger.
    ///	@return	The singleton pointer of the logger.
    static Logger *getInstance();

    ///	@brief	Kills the only instance of the logger.
    static void kill();

    ///	@brief	Set the severity of the logger.
    ///	@param	sev		The new severity of the logger.
    ///	@see	Severity
    void setSeverity( Severity sev );
    
    ///	@brief	Returns	the current severity of the logger.
    ///	@return	The current severity.
    Severity getSeverity() const;

    ///	@brief	Logs a debug message.
    ///	@param	rMessage	The message to log.
    void debug( const String &domain, const String &rMessage );

    ///	@brief	Logs a print message.
    ///	@param	rMessage	The message to log.
    void print( const String &rMessage, PrintMode mode = WithDateTime );

    ///	@brief	Logs a warn message.
    ///	@param	rMessage	The message to log.
    void warn( const String &domain, const String &rMessage );

    ///	@brief	Logs a error message.
    ///	@param	rMessage	The message to log.
    void error( const String &domain, const String &rMessage );

    ///	@brief	Logs a fatal error message.
    ///	@param	rMessage	The message to log.
    void fatal( const String &domain, const String &rMessage );

    ///	@brief	Registers a new log stream.
    ///	@param	pLogStream	A pointer showing to the log stream.
    void registerLogStream( AbstractLogStream *pLogStream );
    
    ///	@brief	Unregisters a registered log stream.
    ///	@param	pLogStream	A pointer showing to the log stream.
    void unregisterLogStream( AbstractLogStream *pLogStream );

private:
    Logger();
    ~Logger();
    String getDateTime();
    String getThreadName();

private:
    //	The Standard log stream.
    class StdLogStream : public AbstractLogStream {
    public:
        StdLogStream();
        ~StdLogStream();
        void write( const String &rMessage );
    };

    static Logger *s_pLogger;

    typedef CPPCore::TArray<AbstractLogStream*> LogStreamArray;
    LogStreamArray m_LogStreams;
};

//-------------------------------------------------------------------------------------------------
void debugPrint( const String &file, int line, const String &message,
    const String &domain = "" );

//-------------------------------------------------------------------------------------------------
void logPrint( const String &file, int line, const String &message,
    const String &domain = "" );

//-------------------------------------------------------------------------------------------------
void warnPrint( const String &file, int line, const String &message,
    const String &domain = "" );

//-------------------------------------------------------------------------------------------------
void errorPrint( const String &file, int line, const String &message,
    const String &domain = "" );

//-------------------------------------------------------------------------------------------------
void fatalPrint( const String &file, int line, const String &message,
    const String &domain = "" );

//-------------------------------------------------------------------------------------------------

} // Namespace Common

const String DomainTag = "";

//-------------------------------------------------------------------------------------------------
///	@fn		osre_debug
///	@brief	This helper macro will write the debug message into the logger.
///	@param	message		The message to log.
//-------------------------------------------------------------------------------------------------
#define osre_debug( message ) ::OSRE::Common::debugPrint(  __FILE__, __LINE__, message, DomainTag );

//-------------------------------------------------------------------------------------------------
///	@fn		osre_log
///	@brief	This helper macro will write the info message into the logger.
///	@param	message		The message to log.
//-------------------------------------------------------------------------------------------------
#define osre_log( message )  ::OSRE::Common::logPrint(    __FILE__, __LINE__, message, DomainTag );

//-------------------------------------------------------------------------------------------------
///	@fn		ce_warn
///	@brief	This helper macro will write a warning into the logger.
///	@param	message		The warning to writhe into the log.
//-------------------------------------------------------------------------------------------------
#define osre_warn( message )  ::OSRE::Common::warnPrint(   __FILE__, __LINE__, message, DomainTag );

//-------------------------------------------------------------------------------------------------
///	@fn		osre_error
///	@brief	This helper macro will write a error into the logger.
///	@param	message		The warning to writhe into the log.
//-------------------------------------------------------------------------------------------------
#define osre_error( message ) ::OSRE::Common::errorPrint(  __FILE__, __LINE__, message, DomainTag );

//-------------------------------------------------------------------------------------------------
///	@fn		ce_fatal
///	@brief	This helper macro will write a fatal error into the logger.
///	@param	message		The warning to writhe into the log.
//-------------------------------------------------------------------------------------------------
#define osre_fatal( message ) ::OSRE::Common::fatalPrint( __FILE__, __LINE__, message, DomainTag );

} // Namespace OSRE
