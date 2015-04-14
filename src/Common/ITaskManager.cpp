#include "Threading/ITaskManager.h"

namespace ZFXCE
{
	namespace Threading
	{
		TaskScheduler *TaskScheduler::s_pInstance = NULL;

		void TaskScheduler::setInstance( TaskScheduler *pInstance )
		{
			s_pInstance = pInstance;
		}
		
		TaskScheduler *TaskScheduler::instance()
		{
			return s_pInstance;
		}

	}
}