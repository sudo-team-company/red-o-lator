//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_KERNEL_CONFIG_H
#define RED_O_LATOR_KERNEL_CONFIG_H

#define DEFAULT_SGPRS_AMOUNT 16

#include <memory>
#include "../util/util.h"

struct KernelConfig {
    uint32_t pgmrsrc1;
    uint32_t pgmrsrc2;
    int ndRangeSizeX;
    int ndRangeSizeY;
    int ndRangeSizeZ;
    int wgSizeX;
    int wgSizeY;
    int wgSizeZ;
    int sgprsnum = DEFAULT_SGPRS_AMOUNT;
    int vgprsnum;
    bool dx10clamp = false;
    bool ieeemode = true;

    // todo
    uint32_t floatmode;
    bool useargs;
    bool usesetup;
    uint8_t priority;

    uint8_t user_sgpr() {
        return uint8_t((pgmrsrc2 >> 1) & 0x1f);
    }

    bool enable_scratch() {
        return get_bit(0, pgmrsrc2) != 0;
    }

    bool enable_idx() {
        return get_bit(7, pgmrsrc2) != 0;
    }

    bool enable_idy() {
        return get_bit(8, pgmrsrc2) != 0;
    }

    bool enable_idz() {
        return get_bit(9, pgmrsrc2) != 0;
    }

    bool enable_sgpr_workgroup_info() {
        return get_bit(10, pgmrsrc2) != 0;
    }

};





#endif  // RED_O_LATOR_KERNEL_CONFIG_H
