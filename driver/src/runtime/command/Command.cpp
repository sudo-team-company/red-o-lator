#include <cassert>
#include <common/utils/time-utils.hpp>

#include "Command.h"
#include "runtime/icd/CLCommandQueue.h"

Command::Command(CLCommandQueue* commandQueue) : commandQueue(commandQueue) {
    clRetainCommandQueue(commandQueue);
}

Command::~Command() {
    assert(waitList.empty());

    if (mEvent) {
        clReleaseEvent(mEvent);
    }

    clReleaseCommandQueue(commandQueue);
}

void Command::execute() {
    assert(mEvent != nullptr);

    if (!mEvent->isQueued()) {
        assert(waitList.empty());
        return;
    }

    if (commandQueue->isOutOfOrder()) {
        while (!waitList.empty()) {
            const auto waitEvent = waitList.top();
            waitList.pop();

            if (waitEvent->isUserEvent && !waitEvent->isCompleted()) {
                // TODO: what is proper behavior?
                continue;
            }

            if (waitEvent->isQueued()) {
                waitEvent->command->execute();
            }

            clReleaseEvent(waitEvent);
        }
    }

    mEvent->setSubmitted();
    mEvent->setRunning();

    executeImpl();

    mEvent->setCompleted();
}

CLEvent* Command::requireEvent() {
    assert(mEvent != nullptr);
    return mEvent;
}

void Command::setEvent(CLEvent* event) {
    this->mEvent = event;
    clRetainEvent(event);
}

void Command::addEventsToWaitList(cl_uint count,
                                  const cl_event* originWaitList) {
    for (size_t i = 0; i < count; ++i) {
        const auto event = originWaitList[i];
        waitList.push(event);
        clRetainEvent(event);
    }
}
