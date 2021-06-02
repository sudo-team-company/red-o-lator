#include <common/utils/optional-utlis.hpp>

#include "icd/icd.h"
#include "runtime-commons.h"

CL_API_ENTRY cl_event CL_API_CALL clCreateUserEvent(cl_context context,
                                                    cl_int* errcode_ret) {
    if (!context) {
        SET_ERROR_AND_RETURN(CL_INVALID_CONTEXT, "Context is null.");
    }

    const auto event = new CLEvent(kDispatchTable, context, kClock, nullptr);
    event->isUserEvent = true;
    event->setSubmitted();

    return event;
}

CL_API_ENTRY cl_int CL_API_CALL clSetUserEventStatus(cl_event event,
                                                     cl_int execution_status) {
    if (!event || !event->isUserEvent) {
        RETURN_ERROR(CL_INVALID_EVENT, "Event is null or not user event.");
    }

    if (execution_status != CL_COMPLETE && execution_status >= 0) {
        RETURN_ERROR(
            CL_INVALID_VALUE,
            "Execution status should be CL_COMPLETE or negative value, got " +
                std::to_string(execution_status) + ".");
    }

    if (event->isCompleted()) {
        RETURN_ERROR(CL_INVALID_OPERATION, "Event is already completed.");
    }

    event->setCompleted(execution_status);

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clRetainEvent(cl_event event) {
    if (!event) {
        RETURN_ERROR(CL_INVALID_EVENT, "Event is null.");
    }

    event->referenceCount++;

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clReleaseEvent(cl_event event) {
    if (!event) {
        RETURN_ERROR(CL_INVALID_EVENT, "Event is null.");
    }

    event->referenceCount--;

    if (event->referenceCount == 0) {
        delete event;
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clWaitForEvents(cl_uint num_events,
                                                const cl_event* event_list) {
    if (!num_events || !event_list) {
        RETURN_ERROR(CL_INVALID_VALUE,
                     "num_events == 0 or event_list is null.");
    }

    for (cl_uint i = 0; i < num_events; ++i) {
        const auto event = event_list[i];

        if (!event) {
            RETURN_ERROR(CL_INVALID_EVENT, "Event at position " +
                                               std::to_string(i) + " is null.");
        }

        if (event->command) {
            // TODO: check the same context
            event->command->execute();
            if (event->getCommandExecutionStatus() < 0) {
                return CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST;
            }
        }
    }

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL
clSetEventCallback(cl_event event,
                   cl_int command_exec_callback_type,
                   CLEventCallbackFunction pfn_notify,
                   void* user_data) {
    if (!event) {
        RETURN_ERROR(CL_INVALID_EVENT, "Event is null.");
    }

    if (!pfn_notify) {
        RETURN_ERROR(CL_INVALID_VALUE, "Callback is null.");
    }

    if (command_exec_callback_type != CL_SUBMITTED &&
        command_exec_callback_type != CL_RUNNING &&
        command_exec_callback_type != CL_COMPLETE) {
        RETURN_ERROR(CL_INVALID_VALUE,
                     "Callback type should be either CL_SUBMITTED, CL_RUNNING "
                     "or CL_COMPLETE, got " +
                         std::to_string(command_exec_callback_type) + ".");
    }

    event->setCallback(command_exec_callback_type,
                       CLEventCallback(pfn_notify, user_data));

    return CL_SUCCESS;
}

CL_API_ENTRY cl_int CL_API_CALL clGetEventInfo(cl_event event,
                                               cl_event_info param_name,
                                               size_t param_value_size,
                                               void* param_value,
                                               size_t* param_value_size_ret) {
    if (!event) {
        RETURN_ERROR(CL_INVALID_EVENT, "Event is null.");
    }

    return getParamInfo(
        param_name, param_value_size, param_value, param_value_size_ret, [&]() {
            CLObjectInfoParameterValueType result;
            size_t resultSize;

            switch (param_name) {
                case CL_EVENT_COMMAND_QUEUE: {
                    result = event->isUserEvent ? nullptr
                                                : event->command->commandQueue;
                    resultSize = sizeof(cl_command_queue);
                    break;
                }

                case CL_EVENT_CONTEXT: {
                    result = event->context;
                    resultSize = sizeof(cl_context);
                    break;
                }

                case CL_EVENT_COMMAND_TYPE: {
                    const auto commandType =
                        event->isUserEvent ? CL_COMMAND_USER
                                           : event->command->getCommandType();
                    result = reinterpret_cast<void*>(commandType);
                    resultSize = sizeof(cl_command_type);
                    break;
                }

                case CL_EVENT_COMMAND_EXECUTION_STATUS: {
                    result = reinterpret_cast<void*>(
                        event->getCommandExecutionStatus());
                    resultSize = sizeof(cl_int);
                    break;
                }

                case CL_EVENT_REFERENCE_COUNT: {
                    result = reinterpret_cast<void*>(event->referenceCount);
                    resultSize = sizeof(cl_uint);
                    break;
                }

                default: return utils::optionalOf<CLObjectInfoParameterValue>();
            }

            return utils::optionalOf(
                CLObjectInfoParameterValue(result, resultSize));
        });
}

CL_API_ENTRY cl_int CL_API_CALL
clGetEventProfilingInfo(cl_event event,
                        cl_profiling_info param_name,
                        size_t param_value_size,
                        void* param_value,
                        size_t* param_value_size_ret) {
    if (!event) {
        RETURN_ERROR(CL_INVALID_EVENT, "Event is null.");
    }

    if (!event->isUserEvent && !event->command) {
        RETURN_ERROR(CL_INVALID_EVENT,
                     "Event is not user event and does not have associated "
                     "command. Probably something wrong in driver "
                     "implementation and this bug should be reported.");
    }

    if (event->isUserEvent ||
        !event->command->commandQueue->isProfilingEnabled()) {
        return CL_PROFILING_INFO_NOT_AVAILABLE;
    }

    if (!event->isCompleted()) {
        return CL_PROFILING_INFO_NOT_AVAILABLE;
    }

    return getParamInfo(
        param_name, param_value_size, param_value, param_value_size_ret, [&]() {
            CLObjectInfoParameterValueType result;
            size_t resultSize = sizeof(cl_ulong);

            switch (param_name) {
                case CL_PROFILING_COMMAND_QUEUED: {
                    result = reinterpret_cast<void*>(event->getTimeQueued());
                    break;
                }

                case CL_PROFILING_COMMAND_SUBMIT: {
                    result = reinterpret_cast<void*>(event->getTimeSubmitted());
                    break;
                }

                case CL_PROFILING_COMMAND_START: {
                    result = reinterpret_cast<void*>(event->getTimeStart());
                    break;
                }

                case CL_PROFILING_COMMAND_END: {
                    result = reinterpret_cast<void*>(event->getTimeEnd());
                    break;
                }

                default: return utils::optionalOf<CLObjectInfoParameterValue>();
            }
            return utils::optionalOf(
                CLObjectInfoParameterValue(result, resultSize));
        });
}
