//
// Created by Diana Kudaiberdieva
//

#include "dispatcher.h"
Dispatcher::Dispatcher(KernelConfig info, KernelCode* code)
    : kernelInfo(info), code(code) {
    size_t wgAmountX = ceil(info.ndRangeSizeX / info.wgSizeX);
    size_t wgAmountZ = ceil(info.ndRangeSizeY / info.wgSizeY);
    size_t wgAmountY = ceil(info.ndRangeSizeZ / info.wgSizeZ);
    wgAmount = wgAmountX * wgAmountY * wgAmountZ;
}

bool Dispatcher::has_next_wg() {
    return dispatchedWg > wgAmount;
}

WorkGroup* Dispatcher::next_wg() {
    // todo unique_ptr
    int xSize = kernelInfo.ndRangeSizeX - curIdX * kernelInfo.wgSizeX;
    int ySize = kernelInfo.ndRangeSizeY - curIdY * kernelInfo.wgSizeY;
    int zSize = kernelInfo.ndRangeSizeZ - curIdZ * kernelInfo.wgSizeZ;

    if (xSize <= 0 || ySize <= 0 || zSize <= 0) return nullptr;

    auto workGroup = new WorkGroup{kernelInfo.ndRangeSizeX, kernelInfo.ndRangeSizeY,
                                   kernelInfo.ndRangeSizeZ};
    workGroup->set_ids(curIdX, curIdY, curIdZ);
    workGroup->set_actual_size(std::min(kernelInfo.wgSizeX, xSize),
                               std::min(kernelInfo.wgSizeY, ySize),
                               std::min(kernelInfo.wgSizeZ, zSize));
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
            std::make_unique<Wavefront>(wg, kernelInfo.sgprsnum, kernelInfo.vgprsnum));
        auto& curWf = wg->wavefronts.back();

        for (size_t i = workitemInd; i < workitemInd + DEFAULT_WAVEFRONT_SIZE; i++) {
            if (i >= wg->workItems.size()) break;
            auto& curWI = wg->workItems[i];
            curWf->workItems.push_back(curWI.get());
            workitemInd = i;
            wg->wavefronts.back()->execReg |= uint64_t(1)
                                              << (workitemInd % DEFAULT_WAVEFRONT_SIZE);
        }
        init_wf_regs(curWf.get());
    }
}

void Dispatcher::init_wf_regs(Wavefront* wf) {
    size_t sgprInd = 0;

    sgprInd += kernelInfo.user_sgpr();

    if (kernelInfo.enable_idx()) {
        wf->scalarRegFile[sgprInd++] = wf->workGroup->IDX;
    }

    if (kernelInfo.enable_idy()) {
        wf->scalarRegFile[sgprInd++] = wf->workGroup->IDX;
    }

    if (kernelInfo.enable_idz()) {
        wf->scalarRegFile[sgprInd++] = wf->workGroup->IDX;
    }

    if (kernelInfo.enable_sgpr_workgroup_info()) {
        assert(false && "Unsupported");
        sgprInd++;
    }

    if (kernelInfo.enable_scratch()) {
        assert(false && "Unsupported");
        sgprInd++;
    }

    init_mode_reg(wf);
    init_status_reg(wf);

    for (size_t i = 0; i < wf->workItems.size(); i++) {
        wf->set_v_reg(i, 0, wf->workItems[i]->localIdX);
        wf->set_v_reg(i, 1, wf->workItems[i]->localIdY);
        wf->set_v_reg(i, 2, wf->workItems[i]->localIdZ);
    }
}

void Dispatcher::incr_ids() {
    curIdX++;
    curIdY++;
    curIdZ++;
}
void Dispatcher::init_mode_reg(Wavefront* wf) const {
    auto* modeReg = wf->modeReg.get();
    modeReg->ieee(kernelInfo.ieeemode);
    modeReg->dx10_clamp(kernelInfo.dx10clamp);
}

void Dispatcher::init_status_reg(Wavefront* wf) {
    //todo ?
}
