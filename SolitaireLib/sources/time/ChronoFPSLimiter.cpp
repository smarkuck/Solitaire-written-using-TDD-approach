#include "interfaces/time/StdTimeFunctionsWrapper.h"
#include "time/ChronoFPSLimiter.h"

using namespace solitaire::time::interfaces;

namespace solitaire::time {

namespace {
constexpr double frameTime {1000 / 60.0};
}

ChronoFPSLimiter::ChronoFPSLimiter(
    std::unique_ptr<StdTimeFunctionsWrapper> stdTimeFunctionsWrapper):
        stdTimeFunctionsWrapper {std::move(stdTimeFunctionsWrapper)},
        frameStartTime {this->stdTimeFunctionsWrapper->now()} {
}

void ChronoFPSLimiter::saveFrameStartTime() {
    frameStartTime = stdTimeFunctionsWrapper->now();
}

void ChronoFPSLimiter::sleepRestOfFrameTime() const {
    std::chrono::duration<double, std::milli> workTime {
        stdTimeFunctionsWrapper->now() - frameStartTime};

    if (workTime.count() < frameTime)
        calculateRestOfFrameTimeAndSleep(workTime);
}

void ChronoFPSLimiter::calculateRestOfFrameTimeAndSleep(
    const std::chrono::duration<double, std::milli>& workTime) const
{
    std::chrono::duration<double, std::milli> restOfTime {frameTime - workTime.count()};

    stdTimeFunctionsWrapper->sleep_for(std::chrono::milliseconds {
        std::chrono::duration_cast<std::chrono::milliseconds>(restOfTime).count()});
}

}
