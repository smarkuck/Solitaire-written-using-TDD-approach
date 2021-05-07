#include <thread>
#include "time/StdTimeFunctionsWrapper.h"

namespace solitaire::time {

std::chrono::system_clock::time_point StdTimeFunctionsWrapper::now() const {
    return std::chrono::system_clock::now();
}

void StdTimeFunctionsWrapper::sleep_for(
    const std::chrono::milliseconds& duration) const
{
    std::this_thread::sleep_for(duration);
}

}
