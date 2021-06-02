#pragma once

#include <common/utils/time-utils.hpp>
#include <stack>
#include <unordered_map>
#include <utility>

#include "icd.h"
#include "runtime/command/Command.h"

using CommandExecutionStatus = cl_int;
using CLEventCallbackFunction = void(CL_CALLBACK*)(cl_event event,
                                                   cl_int event_command_status,
                                                   void* user_data);

struct CLEventCallback {
    CLEventCallback(CLEventCallbackFunction callback, void* userData)
        : function(callback), userData(userData) {}

    CLEventCallbackFunction function;
    void* userData;
};

class Command;

class CLEvent {
   public:
    explicit CLEvent(IcdDispatchTable* dispatchTable,
                     CLContext* context,
                     utils::Clock* clock,
                     Command* command);

    ~CLEvent();

    void setCallback(CommandExecutionStatus status,
                     const CLEventCallback& callback);

    IcdDispatchTable* const dispatchTable;
    Command* const command;
    CLContext* const context;

    bool isUserEvent = false;

    unsigned int referenceCount = 1;

    [[nodiscard]] bool isQueued() const;
    void setQueued();

    [[nodiscard]] bool isSubmitted() const;
    void setSubmitted();

    [[nodiscard]] bool isRunning() const;
    void setRunning();

    [[nodiscard]] bool isCompleted() const;
    void setCompleted(cl_int status = CL_COMPLETE);

    [[nodiscard]] CommandExecutionStatus getCommandExecutionStatus() const;

    [[nodiscard]] cl_ulong getTimeQueued() const;
    [[nodiscard]] cl_ulong getTimeSubmitted() const;
    [[nodiscard]] cl_ulong getTimeStart() const;
    [[nodiscard]] cl_ulong getTimeEnd() const;

   private:
    void callCallback(CommandExecutionStatus status);

    CommandExecutionStatus mCommandExecutionStatus = CL_QUEUED;

    cl_ulong mTimeQueued = 0;
    cl_ulong mTimeSubmitted = 0;
    cl_ulong mTimeStart = 0;
    cl_ulong mTimeEnd = 0;

    utils::Clock* mClock;

    std::unordered_map<cl_int, std::stack<CLEventCallback>> mCallbacks{};
};
