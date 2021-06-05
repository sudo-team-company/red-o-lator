#include <cassert>
#include <common/utils/time-utils.hpp>

#include "CLEvent.h"

CLEvent::CLEvent(IcdDispatchTable* dispatchTable,
                 CLContext* context,
                 utils::Clock* clock,
                 Command* command)
    : dispatchTable(dispatchTable),
      mClock(clock),
      command(command),
      context(context),
      commandQueue(command->commandQueue) {
    clRetainContext(context);
    clRetainCommandQueue(commandQueue);
}

CLEvent::~CLEvent() {
    for (auto& [_, callbacks] : mCallbacks) {
        while (!callbacks.empty()) {
            callbacks.pop();
        }
    }

    mCallbacks.clear();

    clReleaseContext(context);
    clReleaseCommandQueue(commandQueue);
}

void CLEvent::setCallback(CommandExecutionStatus status,
                          const CLEventCallback& callback) {
    mCallbacks[status] = std::stack<CLEventCallback>();
    mCallbacks[status].emplace(callback);
}

void CLEvent::callCallback(CommandExecutionStatus status) {
    const auto statusType = status < 0 ? CL_COMPLETE : status;
    auto callbacks = mCallbacks[statusType];
    while (!callbacks.empty()) {
        const auto callback = callbacks.top();
        callbacks.pop();
        callback.function(this, status, callback.userData);
    }
}

bool CLEvent::isQueued() const {
    return mCommandExecutionStatus == CL_QUEUED;
}

void CLEvent::setQueued() {
    mCommandExecutionStatus = CL_QUEUED;
    mTimeQueued = mClock->now();
}

bool CLEvent::isSubmitted() const {
    return mCommandExecutionStatus == CL_SUBMITTED;
}

void CLEvent::setSubmitted() {
    mCommandExecutionStatus = CL_SUBMITTED;
    mTimeSubmitted = mClock->now();
    callCallback(CL_SUBMITTED);
}

bool CLEvent::isRunning() const {
    return mCommandExecutionStatus == CL_RUNNING;
}

void CLEvent::setRunning() {
    mCommandExecutionStatus = CL_RUNNING;
    mTimeStart = mClock->now();
    callCallback(CL_RUNNING);
}

bool CLEvent::isCompleted() const {
    return mCommandExecutionStatus == CL_COMPLETE ||
           mCommandExecutionStatus < 0;
}

void CLEvent::setCompleted(cl_int status) {
    assert(status == CL_COMPLETE || status < 0);

    mCommandExecutionStatus = status;
    mTimeEnd = mClock->now();
    callCallback(status);
}

CommandExecutionStatus CLEvent::getCommandExecutionStatus() const {
    return mCommandExecutionStatus;
}

cl_ulong CLEvent::getTimeQueued() const {
    return mTimeQueued;
}

cl_ulong CLEvent::getTimeSubmitted() const {
    return mTimeSubmitted;
}

cl_ulong CLEvent::getTimeStart() const {
    return mTimeStart;
}

cl_ulong CLEvent::getTimeEnd() const {
    return mTimeEnd;
}
