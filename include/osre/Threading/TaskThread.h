#ifndef CE_INFRASTRUCTURE_THREADING_TASKTHREAD_H_INC
#define CE_INFRASTRUCTURE_THREADING_TASKTHREAD_H_INC

#ifdef WIN32
#include "Threading/Win32/Win32Thread.h"
#else
#error "Not implemented"
#endif 

namespace ZFXCE
{
namespace Threading
{
#ifdef _WIN32
class TaskThread : public Win32Thread
#else
#error "Not implemented"
#endif
{
public:
	TaskThread();
	~TaskThread();

};

}
}

#endif // CE_INFRASTRUCTURE_THREADING_TASKTHREAD_H_INC
