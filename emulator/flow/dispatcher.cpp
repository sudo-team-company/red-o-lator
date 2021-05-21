//
// Created by Diana Kudaiberdieva
//

#include "dispatcher.h"
Dispatcher::Dispatcher(KernelConfig config, KernelCode* code)
    : kernelConfig(config), code(code) {
    size_t wgAmountX = (config.ndRangeSizeX - 1) / config.wgSizeX + 1;
    size_t wgAmountZ = (config.ndRangeSizeY - 1) / config.wgSizeY + 1;
    size_t wgAmountY = (config.ndRangeSizeZ - 1) / config.wgSizeZ + 1;
    wgAmount = wgAmountX * wgAmountY * wgAmountZ;
}

bool Dispatcher::has_next_wg() const {
    return dispatchedWg < wgAmount;
}

WorkGroup* Dispatcher::next_wg() {
    int xSize = kernelConfig.ndRangeSizeX - curIdX * kernelConfig.wgSizeX;
    int ySize = kernelConfig.ndRangeSizeY - curIdY * kernelConfig.wgSizeY;
    int zSize = kernelConfig.ndRangeSizeZ - curIdZ * kernelConfig.wgSizeZ;

    if (xSize <= 0 || ySize <= 0 || zSize <= 0) return nullptr;

    auto workGroup = new WorkGroup{kernelConfig.ndRangeSizeX, kernelConfig.ndRangeSizeY,
                                   kernelConfig.ndRangeSizeZ};
    workGroup->set_ids(curIdX, curIdY, curIdZ);
    workGroup->set_actual_size(std::min(kernelConfig.wgSizeX, xSize),
                               std::min(kernelConfig.wgSizeY, ySize),
                               std::min(kernelConfig.wgSizeZ, zSize));
    workGroup->kernelCode = code;
    set_workitems(workGroup);
    set_wavefronts(workGroup);
    incr_ids();
    return workGroup;
}
void Dispatcher::set_workitems(WorkGroup* wg) {
    for (int x = 0; x < wg->actualSizeX; x++) {
        for (int y = 0; y < wg->actualSizeY; y++) {
            for (int z = 0; z < wg->actualSizeZ; z++) {
                wg->workItems.push_back(std::make_unique<WorkItem>(wg, x, y, z));
            }
        }
    }
}
void Dispatcher::set_wavefronts(WorkGroup* wg) {
    size_t workitemInd = 0;
    while (workitemInd < wg->workItems.size()) {
        wg->wavefronts.push_back(
            std::make_unique<Wavefront>(wg, kernelConfig.sgprsnum,
                                                             kernelConfig.vgprsnum));
        auto& curWf = wg->wavefronts.back();

        for (size_t i = workitemInd; i < workitemInd + DEFAULT_WAVEFRONT_SIZE; i++) {
            if (i >= wg->workItems.size()) break;
            auto& curWI = wg->workItems[i];
            curWf->workItems.push_back(curWI.get());
            wg->wavefronts.back()->execReg |= uint64_t(1)
                                              << (workitemInd % DEFAULT_WAVEFRONT_SIZE);
            workitemInd = i + 1;
        }
        init_wf_regs(curWf.get());
    }
}

void Dispatcher::init_wf_regs(Wavefront* wf) {
    size_t sgprInd = 0;

    sgprInd += kernelConfig.user_sgpr();

    if (kernelConfig.enable_idx()) {
        wf->scalarRegFile[sgprInd++] = wf->workGroup->IDX;
    }

    if (kernelConfig.enable_idy()) {
        wf->scalarRegFile[sgprInd++] = wf->workGroup->IDX;
    }

    if (kernelConfig.enable_idz()) {
        wf->scalarRegFile[sgprInd++] = wf->workGroup->IDX;
    }

    if (kernelConfig.enable_sgpr_workgroup_info()) {
        assert(false && "Unsupported");
        sgprInd++;
    }

    if (kernelConfig.enable_scratch()) {
        assert(false && "Unsupported");
        sgprInd++;
    }

    if (kernelConfig.usesetup) {
        wf->set_sgpr_pair(4, kernelConfig.kernSetupAddr);
        wf->set_sgpr_pair(6, kernelConfig.kernArgAddr);
    } else if (kernelConfig.useargs) {
        wf->set_sgpr_pair(4, kernelConfig.kernArgAddr);
    }

    init_mode_reg(wf);
    init_status_reg(wf);

    for (size_t i = 0; i < wf->workItems.size(); i++) {
        wf->set_vgpr(i, 0, wf->workItems[i]->localIdX);
        wf->set_vgpr(i, 1, wf->workItems[i]->localIdY);
        wf->set_vgpr(i, 2, wf->workItems[i]->localIdZ);
    }
}

void Dispatcher::incr_ids() {
    curIdX++;
    curIdY++;
    curIdZ++;
    dispatchedWg++;
}
void Dispatcher::init_mode_reg(Wavefront* wf) const {
    auto* modeReg = wf->modeReg.get();
    modeReg->ieee(kernelConfig.ieeemode);
    modeReg->dx10_clamp(kernelConfig.dx10clamp);
}

void Dispatcher::init_status_reg(Wavefront* wf) {
    //todo ?
}
