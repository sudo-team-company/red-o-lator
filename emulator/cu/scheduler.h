//
// Created by Diana Kudaiberdieva
//

#ifndef RED_O_LATOR_SCHEDULER_H
#define RED_O_LATOR_SCHEDULER_H

struct ComputeUnit;

struct Scheduler {
    ComputeUnit* cu;

    Scheduler(ComputeUnit* cu) : cu(cu) {}
};

#endif  // RED_O_LATOR_SCHEDULER_H
