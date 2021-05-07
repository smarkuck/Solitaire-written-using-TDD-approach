#pragma once

#include "interfaces/time/StdTimeFunctionsWrapper.h"

namespace solitaire::time {

class DefaultStdTimeFunctionsWrapper: public interfaces::StdTimeFunctionsWrapper {
public:
    std::chrono::system_clock::time_point now() const override;
    void sleep_for(const std::chrono::milliseconds&) const override;
};

}
