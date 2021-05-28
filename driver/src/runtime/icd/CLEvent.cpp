#include "CLEvent.h"

bool CLEvent::isCompleted() const {
    return commandExecutionStatus == CL_COMPLETE || commandExecutionStatus < 0;
}
bool CLEvent::isQueued() const {
    return commandExecutionStatus == CL_QUEUED;
}
