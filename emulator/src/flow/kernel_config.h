#pragma once

#include <memory>
#include <common/utils/common.hpp>

#include "src/util/util.h"
#include "commons/commons.h"
#include <array>

struct KernelConfig {
public:
    uint64_t kernArgAddr = 0;

    explicit KernelConfig(
        uint32_t dims,
        const size_t *globalWorkOffset,
        const size_t *globalWorkSize,
        const size_t *localWorkSize,
        const std::vector<std::string> &configParams
    );

    size_t get_sgpr_size() const {
        return sgprsnum;
    }

    size_t get_vgpr_size() const {
        return vgprsnum;
    }

    int get_priority() const {
        return priority;
    }

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

    bool ieee_mode_enabled() const {
        return ieeemode;
    }

    bool dx10_clamp_enabled() const {
        return dx10clamp;
    }

    bool use_setup() const {
        return usesetup;
    }

    bool use_generic() const {
        return usegeneric;
    }

    bool use_args() const {
        return useargs;
    }

    size_t get_NDRange_X_size() const {
        return globalWorkSize[0];
    }

    size_t get_NDRange_Y_size() const {
        return dims > 1 ? globalWorkSize[1] : 1;
    }

    size_t get_NDRange_Z_size() const {
        return dims > 2 ? globalWorkSize[2] : 1;
    }

    size_t get_wg_X_size() const {
        return localWorkSize[0];
    }

    size_t get_wg_Y_size() const {
        return dims > 1 ? localWorkSize[1] : 1;
    }

    size_t get_wg_Z_size() const {
        return dims > 2 ? localWorkSize[2] : 1;
    }

private:
    uint32_t dims;
    std::array<size_t, 3> globalWorkOffset{0, 0, 0};
    std::array<size_t, 3> globalWorkSize{1, 1, 1};
    std::array<size_t, 3> localWorkSize{1, 1, 1};
    size_t sgprsnum = 0;
    size_t vgprsnum = 0;
    uint32_t pgmrsrc2 = 0;
    uint8_t priority;
    bool dx10clamp = false;
    bool ieeemode = false;
    bool useargs = false;
    bool usesetup = false;
    bool usegeneric = false;
};