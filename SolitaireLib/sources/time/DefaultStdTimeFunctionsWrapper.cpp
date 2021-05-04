#include <thread>
#include "time/DefaultStdTimeFunctionsWrapper.h"

namespace solitaire::time {

std::chrono::system_clock::time_point DefaultStdTimeFunctionsWrapper::now() const {
    return std::chrono::system_clock::now();
}

void DefaultStdTimeFunctionsWrapper::sleep_for(
    const std::chrono::milliseconds& duration) const
{
    std::this_thread::sleep_for(duration);
}

}
