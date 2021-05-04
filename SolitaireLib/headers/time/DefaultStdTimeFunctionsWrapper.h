#pragma once

#include "StdTimeFunctionsWrapper.h"

namespace solitaire::time {

class DefaultStdTimeFunctionsWrapper: public StdTimeFunctionsWrapper {
public:
    std::chrono::system_clock::time_point now() const override;
    void sleep_for(const std::chrono::milliseconds&) const override;
};

}
