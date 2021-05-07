#pragma once

#include <chrono>
#include <memory>

#include "interfaces/time/FPSLimiter.h"

namespace solitaire::time::interfaces {
class StdTimeFunctionsWrapper;
}

namespace solitaire::time {

class ChronoFPSLimiter: public interfaces::FPSLimiter {
public:
    ChronoFPSLimiter(std::unique_ptr<interfaces::StdTimeFunctionsWrapper>);

    void saveFrameStartTime() override;
    void sleepRestOfFrameTime() const override;

private:
    void calculateRestOfFrameTimeAndSleep(
        const std::chrono::duration<double, std::milli>&) const;

    std::unique_ptr<interfaces::StdTimeFunctionsWrapper> stdTimeFunctionsWrapper;
    std::chrono::system_clock::time_point frameStartTime;
};

}
