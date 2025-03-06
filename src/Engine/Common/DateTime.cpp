/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2025 OSRE ( Open Source Render Engine ) by Kim Kulling

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
#include "Common/DateTime.h"

#include <ctime>

#ifdef OSRE_WINDOWS
#  include "Platform/Windows/MinWindows.h"
#endif // OSRE_WINDOWS

namespace OSRE::Common {

DateTime::DateTime() :
        m_Year(0), m_Month(0), m_Day(0), m_Hour(0), m_Minute(0), m_Seconds(0), m_Milliseconds(0) {
    // empty
}

DateTime::DateTime(ui32 year, ui32 month, ui32 day, ui32 hour, ui32 minute, ui32 seconds, ui32 ms) :
        m_Year(year), m_Month(month), m_Day(day), m_Hour(hour), m_Minute(minute), m_Seconds(seconds), m_Milliseconds(ms) {
    // empty
}

void DateTime::setCurrentYear(ui32 year) {
    m_Year = year;
}

ui32 DateTime::getCurrentYear() const {
    return m_Year;
}

void DateTime::setCurrentMonth(ui32 month) {
    m_Month = month;
}

ui32 DateTime::getCurrentMonth() const {
    return m_Month;
}

void DateTime::setCurrentDay(ui32 day) {
    m_Day = day;
}

ui32 DateTime::getCurrentDay() const {
    return m_Day;
}

void DateTime::setCurrentHour(ui32 hour) {
    m_Hour = hour;
}

ui32 DateTime::getCurrentHour() const {
    return m_Hour;
}

void DateTime::setCurrentMinute(ui32 minute) {
    m_Minute = minute;
}

ui32 DateTime::getCurrentMinute() const {
    return m_Minute;
}

void DateTime::setCurrentSeconds(ui32 seconds) {
    m_Seconds = seconds;
}

ui32 DateTime::getCurrentSeconds() const {
    return m_Seconds;
}

void DateTime::setCurrentMilliseconds(ui32 ms) {
    m_Milliseconds = ms;
}

ui32 DateTime::getCurrentMilliseconds() const {
    return m_Milliseconds;
}

DateTime DateTime::getCurrentUTCTime() {
    DateTime currentDateTime;

#ifdef OSRE_WINDOWS
    SYSTEMTIME systemtime;
    ::GetSystemTime(&systemtime);

    currentDateTime.m_Year = systemtime.wYear;
    currentDateTime.m_Month = systemtime.wMonth;
    currentDateTime.m_Day = systemtime.wDay;
    currentDateTime.m_Hour = systemtime.wHour;
    currentDateTime.m_Minute = systemtime.wMinute;
    currentDateTime.m_Seconds = systemtime.wSecond;
    currentDateTime.m_Milliseconds = systemtime.wMilliseconds;
#else

    time_t t = time(0); // get time now
    tm *now = localtime(&t);
    currentDateTime.m_Year = now->tm_year + 1900;
    currentDateTime.m_Month = now->tm_mon + 1;
    currentDateTime.m_Day = now->tm_mday;
    currentDateTime.m_Hour = now->tm_hour;
    currentDateTime.m_Minute = now->tm_min;
    currentDateTime.m_Seconds = now->tm_sec;
    currentDateTime.m_Milliseconds = 0;

#endif

    return currentDateTime;
}

} // namespace OSRE::Common