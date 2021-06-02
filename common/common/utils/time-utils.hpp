#pragma once

#include <chrono>

using namespace std::chrono;

namespace utils {
class Clock {
   public:
    [[nodiscard]] virtual uint64_t now() const = 0;
};

class SystemClock : public Clock {
   public:
    [[nodiscard]] uint64_t now() const override {
        const time_point<system_clock, nanoseconds> time = system_clock::now();
        return time.time_since_epoch().count();
    }
};
}  // namespace utils
