#pragma once

#include "gmock/gmock.h"
#include "interfaces/time/StdTimeFunctionsWrapper.h"

namespace solitaire::time {

class StdTimeFunctionsWrapperMock: public interfaces::StdTimeFunctionsWrapper {
public:
    MOCK_METHOD(std::chrono::system_clock::time_point, now, (), (const, override));
    MOCK_METHOD(void, sleep_for, (const std::chrono::milliseconds&), (const, override));
};

}
