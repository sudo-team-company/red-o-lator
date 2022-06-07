#include "dispatcher.h"

#include <memory>

Dispatcher::Dispatcher(const KernelConfig *config, const KernelCode *code)
    : kernelConfig(config), code(code) {
    size_t wgAmountX = (config->get_NDRange_X_size() - 1) / config->get_wg_X_size() + 1;
    size_t wgAmountY = (config->get_NDRange_Y_size() - 1) / config->get_wg_Y_size() + 1;
    size_t wgAmountZ = (config->get_NDRange_Z_size() - 1) / config->get_wg_Z_size() + 1;
    wgAmount = wgAmountX * wgAmountY * wgAmountZ;
}

bool Dispatcher::has_next_wg() const {
    return dispatchedWg < wgAmount;
}

std::unique_ptr<WorkGroup> Dispatcher::next_wg() {
    size_t leftWISizeX = kernelConfig->get_NDRange_X_size() - curIdX * kernelConfig->get_wg_X_size();
    size_t leftWISizeY = kernelConfig->get_NDRange_Y_size() - curIdY * kernelConfig->get_wg_Y_size();
    size_t leftWISizeZ = kernelConfig->get_NDRange_Z_size() - curIdZ * kernelConfig->get_wg_Z_size();

    if (leftWISizeX <= 0 || leftWISizeY <= 0 || leftWISizeZ <= 0) return nullptr;

    auto workGroup = std::make_unique<WorkGroup>(code,
                                                 std::min(kernelConfig->get_wg_X_size(), leftWISizeX),
                                                 std::min(kernelConfig->get_wg_Y_size(), leftWISizeY),
                                                 std::min(kernelConfig->get_wg_Z_size(), leftWISizeZ),
                                                 curIdX, curIdY, curIdZ);

    workGroup->init_workitems();
    workGroup->init_wavefronts(*kernelConfig);

    curIdX++;
    if (leftWISizeX - workGroup->sizeX == 0) {
        curIdY++;
        curIdX = 0;
        if (leftWISizeY - workGroup->sizeY == 0) {
            curIdZ++;
            curIdY = 0;
        }
    }

    dispatchedWg++;

    return workGroup;
}