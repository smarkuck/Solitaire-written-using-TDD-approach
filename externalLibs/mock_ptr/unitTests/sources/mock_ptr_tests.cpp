#include "gmock/gmock.h"
#include "mock_ptr.h"

using namespace testing;

namespace {
class DestructionTestMock {
public:
    DestructionTestMock() {
        EXPECT_CALL(*this, destroy());
    }

    ~DestructionTestMock() {
        destroy();
    }

    MOCK_METHOD(void, destroy, (), ());
};

class ExecutionTestMock {
public:
    ExecutionTestMock() {
        EXPECT_CALL(*this, execute());
    }

    MOCK_METHOD(void, execute, (), ());
};
}

class mock_ptr_destructionTest: public Test {
public:
    mock_ptr<DestructionTestMock> mockPtr;
};

TEST_F(mock_ptr_destructionTest, deleteRawPointerIfUniquePointerNotCreated) {}

TEST_F(mock_ptr_destructionTest, deleteRawPointerOnlyOnceIfUniquePointerCreated) {
    mockPtr.make_unique();
}

TEST_F(mock_ptr_destructionTest, throwExceptionOnCreationOfSecondUniquePointer) {
    mockPtr.make_unique();
    EXPECT_THROW(mockPtr.make_unique(), std::runtime_error);
}

class mock_ptr_executionTest: public Test {
public:
    mock_ptr<ExecutionTestMock> mockPtr;
};

TEST_F(mock_ptr_executionTest, asteriskOperatorShouldReturnReferenceToUnderlyingObject) {
    (*mockPtr).execute();
}

TEST_F(mock_ptr_executionTest, uniquePointerShouldContainRawPointerFromMockPtr) {
    mockPtr.make_unique()->execute();
}
