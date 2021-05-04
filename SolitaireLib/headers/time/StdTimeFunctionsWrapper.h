#pragma once

#include <chrono>

namespace solitaire::time {

class StdTimeFunctionsWrapper {
public:
    virtual ~StdTimeFunctionsWrapper() = default;

    virtual std::chrono::system_clock::time_point now() const = 0;
    virtual void sleep_for(const std::chrono::milliseconds&) const = 0;
};

}
