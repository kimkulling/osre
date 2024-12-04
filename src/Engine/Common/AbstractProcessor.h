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
    /// @brief The alias for processor array types.
    using ProcessorArray = ::cppcore::TArray<AbstractProcessor*>;

    /// @brief  The class destructor, virtual.
    virtual ~AbstractProcessor() = default;

    /// @brief  Will add a precondition.
    /// @param[in] precondition   Process which need to get executed before this.
    virtual void addDependency(AbstractProcessor *precondition);

    /// @brief  Will return the list of preconditions.
    /// @return The list of preconditions.
    virtual const ProcessorArray &getPreconditions() const;

    /// @brief  Override this method for your own data processing function.
    /// @return true, if successful, false in case of an error.
    virtual bool execute() = 0;

protected:
    /// @brief  The default constructor.
    AbstractProcessor() = default;

private:
    ProcessorArray mPreconditions;
};

inline void AbstractProcessor::addDependency( AbstractProcessor *precondition ) {
    if ( nullptr != precondition ) {
        mPreconditions.add(precondition);
    }
}

inline const AbstractProcessor::ProcessorArray &AbstractProcessor::getPreconditions() const {
    return mPreconditions;
}

} // Namespace Common
} // Namespace OSRE
