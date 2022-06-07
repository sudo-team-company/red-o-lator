find_package(nng CONFIG REQUIRED)
find_package(Threads REQUIRED)
find_package(Protobuf REQUIRED)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS proto/command.proto proto/response.proto proto/kernel_request.proto proto/memory_request.proto)

include_directories(${PROTOBUF_INCLUDE_DIR})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(COMMON_SOURCES
        common/utils/common.hpp
        common/logger/Logger.cpp
        common/logger/Logger.h
        common/test/doctest.h
        common/utils/string-utils.hpp
        common/utils/vector-utils.hpp
        common/utils/file-utlis.hpp
        common/utils/optional-utlis.hpp
        common/utils/map-utils.hpp
        common/utils/set-utlis.hpp
        common/utils/time-utils.hpp
        ipc/IpcClient.cpp
        ipc/IpcClient.h
        ipc/IpcServer.cpp
        ipc/IpcServer.h
        ipc/util.h
        ipc/ServerJob.h
        debugger/channel/Channel.h
        debugger/breakpoint/Breakpoint.h
        debugger/breakpoint/BreakpointStorage.cpp
        debugger/breakpoint/BreakpointStorage.h
        debugger/breakpoint/WorkGroupBreakpointStorage.cpp
        debugger/breakpoint/WorkGroupBreakpointStorage.h
        model/kernel/arg/KernelArgumentStore.h
        model/kernel/arg/KernelArgumentKind.h
        model/instr/instr_info.cpp
        model/instr/instr_info.h
        model/instr/instruction.cpp
        model/instr/instruction.h
        model/instr/reg_info.h
        model/kernel/Kernel.h
        model/kernel/KernelConfig.h
        common/utils/bit-utils.hpp debugger/message/DebuggerMessage.cpp debugger/message/DebuggerMessage.h debugger/message/OnStop.h debugger/exec/ExecutionContext.h debugger/exec/ExecutionInfo.h debugger/message/Stop.h debugger/message/OnPause.h debugger/message/RequestMemory.h debugger/message/OnRespondMemory.h debugger/message/RequestRegisters.h debugger/message/OnRespondRegisters.h debugger/message/Run.h debugger/message/SetBreakpoint.h model/reg/RegData.h ipc/IPCWrapper.h ipc/IPCWrapper.cpp ipc/proto/KernelProto.h debugger/message/OnException.h)

add_library(red-o-lator-common SHARED ${PROTO_SRCS} ${PROTO_HDRS} ${COMMON_SOURCES})
target_compile_definitions(red-o-lator-common PUBLIC PROTOBUF_USE_DLLS)
target_include_directories(red-o-lator-common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${PROTOBUF_INCLUDE_DIR})
target_link_libraries(red-o-lator-common nng::nng ${PROTOBUF_LIBRARY})
