//
// Created by Diana Kudaiberdieva on 14.05.2021.
//

#ifndef RED_O_LATOR_WF_BUFFER_H
#define RED_O_LATOR_WF_BUFFER_H

#define WF_BUFFER_SIZE 10

#include <array>
#include <memory>
#include "../flow/wavefront.h"

struct WfBuffer {
    std::array<std::unique_ptr<Wavefront>, WF_BUFFER_SIZE> buffer;

   public:
    void addWf()
};

#endif  // RED_O_LATOR_WF_BUFFER_H
