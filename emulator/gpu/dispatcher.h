//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_DISPATCHER_H
#define RED_O_LATOR_DISPATCHER_H


#include "../flow/kernel_config.h"
#include "../flow/wavefront.h"
class Dispatcher {
   private:
    int curIdX = 0;
    int curIdY = 0;
    int curIdZ = 0;
    size_t dispatchedWg = 0;
    size_t wgAmount;
    KernelConfig kernelConfig;
    KernelCode* code;

    void set_workitems(WorkGroup* wg);
    void set_wavefronts(WorkGroup* wg);
    void init_wf_regs(Wavefront*);
    void init_mode_reg(Wavefront*) const;
    void init_status_reg(Wavefront*);
    void incr_ids();

   public:
    Dispatcher(KernelConfig config, KernelCode* code);
    bool has_next_wg() const;
    WorkGroup* next_wg();
};

#endif  // RED_O_LATOR_DISPATCHER_H
