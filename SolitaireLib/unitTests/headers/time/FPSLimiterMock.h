#pragma once

#include "gmock/gmock.h"
#include "interfaces/time/FPSLimiter.h"

namespace solitaire::time {

class FPSLimiterMock: public interfaces::FPSLimiter {
public:
    MOCK_METHOD(void, saveFrameStartTime, (), (override));
    MOCK_METHOD(void, sleepRestOfFrameTime, (), (const, override));
};

}
