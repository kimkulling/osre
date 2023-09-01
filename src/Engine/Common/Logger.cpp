/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2023 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include <osre/Common/DateTime.h>
#include <osre/Common/Logger.h>
#include <osre/Debugging/Debug.h>
#include <cassert>

#ifdef OSRE_WINDOWS
#    include <src/Engine/Platform/win32/Win32DbgLogStream.h>
#endif // OSRE_WINDOWS

#include <iomanip>
#include <iostream>
#include <sstream>

namespace OSRE {
namespace Common {

static constexpr c8 Line[] =
        "====================================================================================================";

static void appendDomain(const String &domain, String &logMsg) {
    if (!domain.empty()) {
        logMsg += "( ";
        logMsg += domain;
        logMsg += " )";
    }
}

AbstractLogStream::AbstractLogStream() :
        m_IsActive(true) {
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

Logger *Logger::sLogger = nullptr;

Logger *Logger::create() {
    if (nullptr == sLogger) {
        sLogger = new Logger;
    }

    return sLogger;
}

Logger *Logger::getInstance() {
    if (nullptr == sLogger) {
        static_cast<void>(Logger::create());
    }

    return sLogger;
}

void Logger::kill() {
    if (sLogger) {
        delete sLogger;
        sLogger = nullptr;
    }
}

void Logger::setVerboseMode(VerboseMode sev) {
    mVerboseMode = sev;
}

Logger::VerboseMode Logger::getVerboseMode() const {
    return mVerboseMode;
}

void Logger::trace(const String &domain, const String &msg) {
    if (getVerboseMode() == VerboseMode::Trace) {
        String logMsg;
        logMsg += "Trace: ";
        logMsg += msg;
        appendDomain(domain, logMsg);

        print(logMsg);        
    }
}

void Logger::debug(const String &domain, const String &msg) {
    if (getVerboseMode() == VerboseMode::Debug || getVerboseMode() == VerboseMode::Trace) {
        String logMsg;
        logMsg += "Dbg:  ";
        logMsg += msg;
        appendDomain(domain, logMsg);

        print(logMsg);
    }
}

void Logger::info(const String &domain, const String &msg) {
    if (getVerboseMode() == VerboseMode::Normal || getVerboseMode() == VerboseMode::Verbose || getVerboseMode() == VerboseMode::Debug || getVerboseMode() == VerboseMode::Trace) {
        String logMsg;

        logMsg += "Info: ";
        logMsg += msg;

        appendDomain(domain, logMsg);

        print(logMsg);
    }
}

void Logger::print(const String &msg, PrintMode mode) {
    if (msg.empty()) {
        return;
    }

    if (msg.size() > 8) {
        if (msg[6] == '<' && msg[7] == '=') {
            mIntention -= 2;
        }
    }

    String logMsg;
    if (0 != mIntention) {
        for (ui32 i = 0; i < mIntention; i++) {
            logMsg += " ";
        }
    }

    logMsg += msg;
    if (PrintMode::WithDateTime == mode) {
        logMsg += " ( ";
        logMsg += this->getDateTime();
        logMsg += " )";
    }

    logMsg += " \n";
    for (ui32 i = 0; i < mLogStreams.size(); ++i) {
        AbstractLogStream *stream = mLogStreams[i];
        if (stream) {
            stream->write(logMsg);
        }
    }

    if (msg.size() > 8) {
        if (msg[6] == '=' && msg[7] == '>') {
            mIntention += 2;
        }
    }
}

void Logger::warn(const String &domain, const String &msg) {
    String logMsg;
    logMsg += "Warn: ";
    logMsg += msg;
    appendDomain(domain, logMsg);

    print(logMsg);
}

void Logger::error(const String &domain, const String &msg) {
    String logMsg;
    logMsg += "Err:  ";
    logMsg += msg;
    appendDomain(domain, logMsg);

    print(logMsg);
}

void Logger::fatal(const String &domain, const String &msg) {
    String logMsg;
    logMsg += "Fatal:";
    logMsg += msg;
    appendDomain(domain, logMsg);

    print(logMsg);
}

void Logger::registerLogStream(AbstractLogStream *pLogStream) {
    if (nullptr == pLogStream) {
        return;
    }

    mLogStreams.add(pLogStream);
}

void Logger::unregisterLogStream(AbstractLogStream *logStream) {
    if (nullptr != logStream) {
        return;
    }

    for (ui32 i = 0; i < mLogStreams.size(); ++i) {
        if (mLogStreams[i] == logStream) {
            delete mLogStreams[i];
            mLogStreams.remove(i);
        }
    }
}

Logger::Logger() :
        mLogStreams(), 
        mVerboseMode(VerboseMode::Normal),
        mIntention(0) {
    mLogStreams.add(new StdLogStream);

#ifdef OSRE_WINDOWS
    mLogStreams.add(new Platform::Win32DbgLogStream);
#endif // OSRE_WINDOWS

    print(Line, PrintMode::WhithoutDateTime);
    print("New OSRE run.");
    print(Line, PrintMode::WhithoutDateTime);
}

Logger::~Logger() {
    print(Line, PrintMode::WhithoutDateTime);
    print("OSRE run ended.");
    print(Line, PrintMode::WhithoutDateTime);

    for (ui32 i = 0; i < mLogStreams.size(); ++i) {
        delete mLogStreams[i];
    }
}

String Logger::getDateTime() {
    static const ui32 Space = 2;
    DateTime currentDateTime = DateTime::getCurrentUTCTime();
    std::stringstream stream;
    stream.fill('0');
    stream << std::setw(Space) << currentDateTime.getCurrentMonth() << "."
           << std::setw(Space) << currentDateTime.getCurrentDay() << "."
           << std::setw(Space * 2) << currentDateTime.getCurrentYear() << " "
           << std::setw(Space) << currentDateTime.getCurrentHour() << ":"
           << std::setw(Space) << currentDateTime.getCurrentMinute() << ":"
           << std::setw(Space) << currentDateTime.getCurrentSeconds() << " ";

    return stream.str();
}

Logger::StdLogStream::StdLogStream() {
    // empty
}

Logger::StdLogStream::~StdLogStream() {
    // empty
}

void Logger::StdLogStream::write(const String &msg) {
    std::cout << msg;
}

void tracePrint(const String &domain, const String &file, int line, const String &msg) {
    String message;
    message += msg;
    message += " (";
    message += file;
    message += ", ";
    std::stringstream ss;
    ss << line;
    message += ss.str();
    message += ")";
    Logger::getInstance()->trace(domain, message);
}

void debugPrint(const String &domain, const String &file, int line, const String &msg) {
    String message;
    message += msg;
    message += " (";
    message += file;
    message += ", ";
    std::stringstream ss;
    ss << line;
    message += ss.str();
    message += ")";
    Logger::getInstance()->debug(domain, message);
}

void infoPrint(const String &domain, const String &file, int line, const String &msg) {
    String message;
    message += msg;
    message += " (";
    message += file;
    message += ", ";
    std::stringstream ss;
    ss << line;
    message += ss.str();
    message += ")";
    Logger::getInstance()->info(domain, message);
}

void logPrint(const String &domain, const String &file, int line, const String &message) {
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
    Logger::getInstance()->print(tmp);
}

void warnPrint(const String &domain, const String &file, int line, const String &message) {
    String msg;
    msg += message;
    msg += " (";
    msg += file;
    msg += ", ";
    std::stringstream ss;
    ss << line;
    msg += ss.str();
    msg += ")";
    Logger::getInstance()->warn(domain, msg);
}

void errorPrint(const String &domain, const String &file, int line, const String &message) {
    String msg;
    msg += message;
    msg += " (";
    msg += file;
    msg += ", ";
    std::stringstream ss;
    ss << line;
    msg += ss.str();
    msg += ")";
    Logger::getInstance()->error(domain, msg);
}

void fatalPrint(const String &domain, const String &file, int line, const String &message) {
    String msg;
    msg += message;
    msg += " (";
    msg += file;
    msg += ", ";
    std::stringstream ss;
    ss << line;
    msg += ss.str();
    msg += ")";
    Logger::getInstance()->fatal(domain, msg);
}

} // Namespace Common
} // Namespace OSRE
