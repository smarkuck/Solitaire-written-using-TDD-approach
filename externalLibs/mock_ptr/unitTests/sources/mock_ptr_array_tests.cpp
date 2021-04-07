#include "gmock/gmock.h"
#include "mock_ptr_array.h"

using namespace testing;

namespace {
class ExecutionTestBase {
public:
    virtual ~ExecutionTestBase() = default;
    virtual void execute() = 0;
};

class ExecutionTestMock: public ExecutionTestBase {
public:
    ExecutionTestMock() {
        EXPECT_CALL(*this, execute());
    }

    MOCK_METHOD(void, execute, (), (override));
};
}

TEST(mock_ptr_array_test, uniquePointersShouldContainRawPointersFromMockPtrArray) {
    mock_ptr_array<ExecutionTestMock, 3> mockPtrs;
    std::array<std::unique_ptr<ExecutionTestBase>, 3> uniquePtrs =
        mockPtrs.make_unique<ExecutionTestBase>();

    for (const auto& uniquePtr: uniquePtrs)
        (*uniquePtr).execute();
}
