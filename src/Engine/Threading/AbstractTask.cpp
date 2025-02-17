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
#include "Platform/Threading.h"
#include "Threading/AbstractTask.h"

namespace OSRE {
namespace Threading {

AbstractTask::AbstractTask(const String &taskName) :
        Object(taskName), mRefCount(nullptr), mParent(nullptr) {
    mRefCount = new Platform::AtomicInt(0);
}

AbstractTask::~AbstractTask() {
    delete mRefCount;
}

bool AbstractTask::preExecute() {
    return true;
}

bool AbstractTask::postExecute() {
    if (mParent) {
        mParent->removeDependency();
    }

    return true;
}

void AbstractTask::addDependency() {
    mRefCount->incValue(1);
}

void AbstractTask::removeDependency() {
    if (mRefCount->getValue() > 0) {
        mRefCount->decValue(1);
    }
}

size_t AbstractTask::getNumDependencies() const {
    if (mRefCount) {
        const ui32 numRefs = mRefCount->getValue();
        return numRefs;
    } 
    return 0;
}

AbstractTask *AbstractTask::getParent() const {
    return mParent;
}

void AbstractTask::setParent(AbstractTask *pParent) {
    mParent = pParent;
    if (mParent) {
        mParent->addDependency();
    }
}

void AbstractTask::enqueue(AbstractTask *pTask) {
    if (pTask) {
        pTask->setParent(this);
        mChildrenTasks.add(pTask);
    }
}

size_t AbstractTask::getNumChildTasks() const {
    return mChildrenTasks.size();
}

AbstractTask *AbstractTask::getChildTask(ui32 idx) const {
    return mChildrenTasks[idx];
}

} // Namespace Threading
} // Namespace OSRE
