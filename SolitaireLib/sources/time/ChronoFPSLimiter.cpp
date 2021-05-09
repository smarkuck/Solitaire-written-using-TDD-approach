#include "interfaces/time/StdTimeFunctionsWrapper.h"
#include "time/ChronoFPSLimiter.h"

using namespace solitaire::time::interfaces;

namespace solitaire::time {

ChronoFPSLimiter::ChronoFPSLimiter(
    const unsigned fps,
    std::unique_ptr<StdTimeFunctionsWrapper> stdTimeFunctionsWrapper):
        frameTime {1000.0 / fps},
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
