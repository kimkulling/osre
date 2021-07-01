#include <osre/Common/CommandQueue.h>

namespace OSRE {
namespace Common {

CommandQueue::CommandQueue() :
        mCommandQueue() {
    // empty
}
CommandQueue::~CommandQueue() {
    // empty
}

void CommandQueue::enqueue( Command *cmd ) {
    mCommandQueue.enqueue(cmd);
}

bool CommandQueue::isEmpty()  {
    return mCommandQueue.isEmpty();
}

Command *CommandQueue::dequeue() {
    if (isEmpty()) {
        return nullptr;
    }

    return mCommandQueue.dequeue();
}

void CommandQueue::clear() {
    mCommandQueue.clear();
}

}
} // namespace OSRE