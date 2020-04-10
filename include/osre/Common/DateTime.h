/*-----------------------------------------------------------------------------------------------
The MIT License (MIT)

Copyright (c) 2015-2020 OSRE ( Open Source Render Engine ) by Kim Kulling

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

namespace OSRE {
namespace Common {

//-------------------------------------------------------------------------------------------------
///	@ingroup    Engine
///
///	@brief	A utility class to manage dates and times.
//-------------------------------------------------------------------------------------------------
class OSRE_EXPORT DateTime {
public:
    ///	@brief	The class default constructor.
    DateTime();

    ///	@brief	The class constructor with date and time information.
    ///	@param	year		[in] The current year.
    ///	@param	month		[in] The current month.
    ///	@param	day			[in] The current day.
    ///	@param	hour		[in] The current hour.
    ///	@param	minute		[in] The current minute.
    ///	@param	seconds		[in] The current seconds.
    ///	@param	ms			[in] The current milli seconds.
    DateTime( ui32 year, ui32 month, ui32 day, ui32 hour, ui32 minute, ui32 seconds, ui32 ms );

    ///	@brief	The class destructor.
    ~DateTime();

    ///	@brief	Set the new current year.
    ///	@param	year	[in] The new current year
    void setCurrentYear( ui32 year );
    
    ///	@brief	Returns the current year.
    ///	@return	The current year.
    ui32 getCurrentYear() const;

    ///	@brief	Set the new current month.
    ///	@param	month	[in] The new current month.
    void setCurrentMonth( ui32 month );

    ///	@brief	Returns the current year.
    ///	@return	The current month.
    ui32 getCurrentMonth() const;

    ///	@brief	Set the new current day.
    ///	@param	day		[in] The new current day.
    void setCurrentDay( ui32 day );

    ///	@brief	Returns the current year.
    ///	@return	The current day.
    ui32 getCurrentDay() const;

    ///	@brief	Set the new current hour.
    ///	@param	hour	[in] The new current hour.
    void setCurrentHour( ui32 hour );

    ///	@brief	Returns the current hour.
    ///	@return	The current hour.
    ui32 getCurrentHour() const;

    ///	@brief	Set the new current minute.
    ///	@param	minute	[in] The new current minute.
    void setCurrentMinute( ui32 minute );

    ///	@brief	Returns the minute.
    ///	@return	The current minute.
    ui32 getCurrentMinute() const;

    ///	@brief	Set the new current seconds.
    ///	@param	seconds		[in] The new current seconds.
    void setCurrentSeconds( ui32 seconds );

    ///	@brief	Returns the current seconds.
    ///	@return	The current seconds.
    ui32 getCurrentSeconds() const;

    ///	@brief	Set the new current ms.
    ///	@param	ms		[in] The new current ms.
    void setCurrentMilliseconds( ui32 ms );

    ///	@brief	Returns the current ms.
    ///	@return	The current ms.
    ui32 getCurrentMilliseconds() const;

    ///	@brief	Generates the current UTC-time and returns a date-time instance with it.
    ///	@return	The DateTime.
    static DateTime getCurrentUTCTime();

private:
    ui32 m_Year;
    ui32 m_Month;
    ui32 m_Day;
    ui32 m_Hour;
    ui32 m_Minute;
    ui32 m_Seconds;
    ui32 m_Milliseconds;
};

} // Namespace Common
} // Namespace OSRE
