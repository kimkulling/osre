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

#include <cppcore/Container/TArray.h>

namespace OSRE {
namespace Common {
        
//-------------------------------------------------------------------------------------------------
///	@ingroup	Engine
///
///	@brief  This abstract class is used to define any kind of data processors. Each processor stores 
/// its list of preconditions, which can be resolved by performing these processes.
//-------------------------------------------------------------------------------------------------
class AbstractProcessor {
public:
    using ProcessorArray = ::CPPCore::TArray<AbstractProcessor*>;

    /// @brief  The class destructor, virtual.
    virtual ~AbstractProcessor();

    /// @brief  Will add a precondition.
    /// @param  precondition    [in] Process which need to get executed before this.
    virtual void addDependency( AbstractProcessor *precondition );

    /// @brief  Will return the list of preconditions.
    /// @return The list of pre-condtions.
    virtual const ProcessorArray &getPreconditions() const;

    /// @brief  Override this method for your own data processing function.
    /// @return true, if successful, false in case of an error.
    virtual bool execute() = 0;

protected:
    /// @brief  The default constructor.
    AbstractProcessor();

private:
    ProcessorArray m_preconditions;
};

inline
AbstractProcessor::AbstractProcessor() 
: m_preconditions() {
    // empty
}

inline
AbstractProcessor::~AbstractProcessor() {
    // empty    
}

inline
void AbstractProcessor::addDependency( AbstractProcessor *precondition ) {
    if ( nullptr != precondition ) {
        m_preconditions.add( precondition );
    }
}

inline
const AbstractProcessor::ProcessorArray &AbstractProcessor::getPreconditions() const {
    return m_preconditions;
}

} // Namespace Common
} // Namespace OSRE
