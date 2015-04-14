#pragma once

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

* Neither the name of the ASSIMP team, nor the names of its contributors may be used to endorse or 
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
#ifndef ZFXCE2_INFRASTRUCTURE_Common_ITASK_H_INC
#define ZFXCE2_INFRASTRUCTURE_Common_ITASK_H_INC

#include <osre2/Infrastructure/Common/Object.h>
#include <cppCommon/Container/TArray.h>

namespace ZFXCE2 {

namespace Common {
    class Event;
    class EventData;
    class AbstractEventHandler;
}

namespace Platform {
    class AtomicInt;
    class AbstractThread;
}

namespace Threading {

//-------------------------------------------------------------------------------------------------
///	@class		::ZFXCE2::Threading::AbstractTask
///	@ingroup	Infrastructure
///
///	@brief	This abstract interface-class declares a task. Tasks can be used to handle several 
///	component jobs like streaming resource loader requests or the rendering. A task can work as a 
///	sequence. Then he will be started in the main thread context. Or the task can be started in a 
///	separate thread. You can install an own thread instance as well. To implement the behavior of 
///	the task you can install event-handlers. Each event with its data will be managed by a task 
///	job. TaskJobs will be queued and handled one after another. You can specify your own 
///	setup-method, if you want to add your own setup code.
//-------------------------------------------------------------------------------------------------
class DLL_EXPORT AbstractTask : public Common::Object {
public:
    ///	@enum	WorkingMode
    ///	@brief	This enum describes the current working mode of the task. Tasks can be in a parallel
    ///			Working mode ( another thread ) or in sequence mode ( runs in the main thread ).
    enum WorkingMode {
        Sync,	///< Runs in the main thread.
        Async	///< Runs in its own thread.
    };

    /// @enum   BufferMode
    /// @brief  Describes the buffered mode for enqueued jobs.
    enum BufferMode {
        SingleBuffer,   ///< Single buffer. enqueued data will be handled in this frame.
        DoubleBuffer    ///< Double buffer. enqueued data will be handled in next frame.
    };

public:
    ///	@brief	The class destructor, virtual.
    virtual ~AbstractTask();

    ///	@brief	Set the current working mode. The task must not be in running mode.
    ///	@param	mode	    [in] The new async working mode.
    virtual void setWorkingMode( WorkingMode mode ) = 0;
    
    ///	@brief	The current working mode will be returned.
    ///	@return	The current working mode.
    virtual WorkingMode getWorkingMode() const = 0;

    /// @brief  Will set a new buffer mode.
    /// @param  buffermode  [in] The new buffer mode.
    virtual void setBufferMode( BufferMode buffermode ) = 0;

    /// @brief  Will return the current buffermode.
    /// @return The current buffer mode.
    virtual BufferMode getBufferMode() const = 0;

    /// @brief  The pre-execution callback, use this for pre-handlers.
    /// @return true, if successful.
    virtual bool preExecute();

    /// @brief  The execution callback.
    /// @return true, if successful.
    virtual bool execute() = 0;

    /// @brief  The post-execution callback, use this for post-handlers.
    /// @return true, if successful.
    virtual bool postExecute();

    ///	@brief	A new thread instance will be assigned to the task. The task must be stopped.
    ///	@param	pThreadInstance		[in] A pointer showing to the current thread instance.
    virtual void setThreadInstance( Platform::AbstractThread *pThreadInstance ) = 0;

    ///	@brief	The update callback.
    virtual void onUpdate() = 0;

    ///	@brief	Awaits an incoming signal.
    virtual void await() = 0;

public:
    /// @brief  Increments number of references.
    void addDependency();

    /// @brief  Decrements the number of references.
    void removeDependency();

    /// @brief  Will return the number of references.
    /// @return The number of references.
    ui32 getNumRefs() const;
    
    /// @brief  Will return the parent task.
    /// @return The parent task, nullptr if root.
    AbstractTask *getParent() const;
    
    /// @brief  Assigns a parent task.
    /// @param  pParent     [in] The new parent task.
    void setParent( AbstractTask *pParent );
    
    /// @brief  Enqueues another task.
    /// @param  pTask       [in] The new task to enqueue.
    void enqueue( AbstractTask *pTask );
    
    /// @brief  Will return the number of enqueued child tasks.
    /// @return The number of enqueued child tasks.
    ui32 getNumChildTasks() const;

    /// @brief  Returns the child task by its index.
    /// @param  idx         [in] The child task index.
    /// @return A pointer showing to the child task or nullptr in case of a range overflow.
    AbstractTask *getChildTask( ui32 idx ) const;

protected:
    ///	@brief	The class constructor with the name of the task.
    ///	@param	taskName		[in] The task-name.
    AbstractTask( const ce_string &taskName );

private:
    Platform::AtomicInt *m_pRefCount;
    AbstractTask *m_pParent;
    CPPCommon::TArray<AbstractTask*> m_childTasks;
};

//-------------------------------------------------------------------------------------------------

} // Namespace Threading
} // Namespace ZFXCE2

#endif // CE_INFRASTRUCTURE_Common_ITASK_H_INC
