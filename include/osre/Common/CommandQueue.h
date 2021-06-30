#pragma once

#include <osre/Common/osre_common.h>
#include <osre/Common/TFunctor.h>
#include <osre/Common/Object.h>
#include <osre/Threading/TAsyncQueue.h>

namespace OSRE {
namespace Common {


using Command = Functor<void, const c8*, void*>;

class CommandQueue {
public:
    CommandQueue();
    ~CommandQueue();
    void enqueue(Command *cmd);
    bool isEmpty();
    Command *dequeue();
    void clear();

private:
    Threading::TAsyncQueue<Command*> mCommandQueue;
};
}
}