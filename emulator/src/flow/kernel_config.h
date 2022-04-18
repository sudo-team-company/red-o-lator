#pragma once

#define DEFAULT_SGPRS_AMOUNT 16

#include <memory>
#include "src/util/util.h"

struct KernelConfig {
    uint64_t kernArgAddr = 0;
    uint64_t kernSetupAddr = 0;
    uint32_t pgmrsrc1;
    uint32_t pgmrsrc2;
    uint32_t floatmode;
    int ndRangeSizeX;
    int ndRangeSizeY;
    int ndRangeSizeZ;
    int wgSizeX;
    int wgSizeY;
    int wgSizeZ;
    int sgprsnum = DEFAULT_SGPRS_AMOUNT;
    int vgprsnum;
    uint8_t priority;
    bool dx10clamp = false;
    bool ieeemode = false;
    bool useargs = false;
    bool usesetup = false;

    uint8_t get_user_sgpr() const {
        return uint8_t((pgmrsrc2 >> 1) & 0x1f);
    }

    bool is_enabled_scratch() const {
        return get_bit(0, pgmrsrc2) != 0;
    }

    bool is_enabled_idx() const {
        return get_bit(7, pgmrsrc2) != 0;
    }

    bool is_enabled_idy() const {
        return get_bit(8, pgmrsrc2) != 0;
    }

    bool is_enabled_idz() const {
        return get_bit(9, pgmrsrc2) != 0;
    }

    bool is_enabled_sgpr_workgroup_info() const {
        return get_bit(10, pgmrsrc2) != 0;
    }
};