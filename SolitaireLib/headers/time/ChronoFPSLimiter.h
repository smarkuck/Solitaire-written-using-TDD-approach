#pragma once

#include <chrono>
#include <memory>

#include "FPSLimiter.h"

namespace solitaire::time {

class StdTimeFunctionsWrapper;

class ChronoFPSLimiter: public FPSLimiter {
public:
    ChronoFPSLimiter(std::unique_ptr<StdTimeFunctionsWrapper>);

    void saveFrameStartTime() override;
    void sleepRestOfFrameTime() const override;

private:
    void calculateRestOfFrameTimeAndSleep(
        const std::chrono::duration<double, std::milli>&) const;

    std::unique_ptr<StdTimeFunctionsWrapper> stdTimeFunctionsWrapper;
    std::chrono::system_clock::time_point frameStartTime;
};

}
