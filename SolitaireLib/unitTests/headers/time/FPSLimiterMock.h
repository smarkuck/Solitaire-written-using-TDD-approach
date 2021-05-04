#pragma once

#include "gmock/gmock.h"
#include "time/FPSLimiter.h"

namespace solitaire::time {

class FPSLimiterMock: public FPSLimiter {
public:
    MOCK_METHOD(void, saveFrameStartTime, (), (override));
    MOCK_METHOD(void, sleepRestOfFrameTime, (), (const, override));
};

}
