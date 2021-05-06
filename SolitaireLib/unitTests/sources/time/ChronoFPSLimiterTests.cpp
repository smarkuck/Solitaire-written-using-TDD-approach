#include "mock_ptr.h"
#include "gmock/gmock.h"
#include "time/ChronoFPSLimiter.h"
#include "time/StdTimeFunctionsWrapperMock.h"

using namespace testing;
using namespace solitaire::time;

namespace solitaire {

namespace {
using Milliseconds = unsigned;
constexpr Milliseconds start {3};
constexpr Milliseconds frameTime {16};
}

class ChronoFPSLimiterTests: public Test {
public:
    void expectGetActualTime(const Milliseconds ms) {
        EXPECT_CALL(*stdTimeFunctionsWrapperMock, now())
            .WillOnce(Return(getMillisecondsTimepoint(ms)));
    }

    void expectSleepForCalculatedRestOfTime(
        const Milliseconds start, const Milliseconds stop)
    {
        const int restOfTime = frameTime - (stop - start);

        if (restOfTime < 0)
            EXPECT_CALL(*stdTimeFunctionsWrapperMock, sleep_for(_)).Times(0);
        else
            EXPECT_CALL(*stdTimeFunctionsWrapperMock,
                sleep_for(std::chrono::milliseconds {restOfTime}));
    }

    std::chrono::time_point<std::chrono::system_clock>
    getMillisecondsTimepoint(const Milliseconds ms) {
        return std::chrono::time_point<std::chrono::system_clock>(
            std::chrono::milliseconds(ms));
    }

    InSequence seq;
    mock_ptr<StdTimeFunctionsWrapperMock> stdTimeFunctionsWrapperMock;
};

TEST_F(ChronoFPSLimiterTests, sleepRestOfFrameTimeSavedDuringConstruction) {
    constexpr Milliseconds stop {5};

    expectGetActualTime(start);
    ChronoFPSLimiter fpsLimiter {stdTimeFunctionsWrapperMock.make_unique()};

    expectGetActualTime(stop);
    expectSleepForCalculatedRestOfTime(start, stop);
    fpsLimiter.sleepRestOfFrameTime();
}

class ConstructedChronoFPSLimiterTests: public ChronoFPSLimiterTests {
public:
    ConstructedChronoFPSLimiterTests() {
        expectGetActualTime(start);

        fpsLimiter = std::make_unique<ChronoFPSLimiter>(
            stdTimeFunctionsWrapperMock.make_unique());
    }

    std::unique_ptr<ChronoFPSLimiter> fpsLimiter;
};

TEST_F(ConstructedChronoFPSLimiterTests, sleepRestOfFrameTimeSavedUsingSaveMethod) {
    constexpr Milliseconds start {4}, stop {5};

    expectGetActualTime(start);
    fpsLimiter->saveFrameStartTime();

    expectGetActualTime(stop);
    expectSleepForCalculatedRestOfTime(start, stop);
    fpsLimiter->sleepRestOfFrameTime();
}

TEST_F(ConstructedChronoFPSLimiterTests,
       sleepZeroMillisecondsIfSleepSetSixteenMillisecondsLater)
{
    constexpr Milliseconds stop {19};

    expectGetActualTime(stop);
    expectSleepForCalculatedRestOfTime(start, stop);
    fpsLimiter->sleepRestOfFrameTime();
}

TEST_F(ConstructedChronoFPSLimiterTests,
       dontSleepIfSleepSetMoreThanSixteenMillisecondsLater)
{
    constexpr Milliseconds stop {20};

    expectGetActualTime(stop);
    expectSleepForCalculatedRestOfTime(start, stop);
    fpsLimiter->sleepRestOfFrameTime();
}

}
