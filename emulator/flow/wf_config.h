#ifndef RED_O_LATOR_WF_CONFIG_H
#define RED_O_LATOR_WF_CONFIG_H

#define SGPRS_NUM_DEFAULT = 16 //https://github.com/ROCm-Developer-Tools/ROCm-ComputeABI-Doc/blob/master/AMDGPU-ABI.md#introduction

// todo
struct WfConfig {
    int sgprsnum;
    int vgprsnum;
    bool dx10clamp;
    bool ieeemode;

    WfConfig(int sgprsnum = SGPRS_NUM_DEFAULT,
             int vgprsnum,
             bool dx10clamp = false,
             bool ieemode = false)
        : sgprsnum(sgprsnum),
          vgprsnum(vgprsnum),
          dx10clamp(dx10clamp),
          ieeemode(ieemode) {}
};

#endif  // RED_O_LATOR_WF_CONFIG_H
