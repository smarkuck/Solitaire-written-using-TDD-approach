#include <utility>

#include "DefaultContext.h"
#include "mock_ptr.h"
#include "SolitaireMock.h"
#include "cards/Card.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace solitaire::geometry;

namespace solitaire {

TEST(DefaultContextTests, setAndGet) {
    mock_ptr<SolitaireMock> solitaireMock;
    DefaultContext context {solitaireMock.make_unique()};

    EXPECT_EQ(&context.getSolitaire(), solitaireMock.get());
    EXPECT_EQ(&std::as_const(context).getSolitaire(), solitaireMock.get());

    Position position1 {1, 7};
    Position position2 {2, 4};

    context.setMousePosition(position1);
    EXPECT_EQ(context.getMousePosition(), position1);
    context.setMousePosition(position2);
    EXPECT_EQ(context.getMousePosition(), position2);
    context.setCardsInHandPosition(position1);
    EXPECT_EQ(context.getCardsInHandPosition(), position1);
    context.setCardsInHandPosition(position2);
    EXPECT_EQ(context.getCardsInHandPosition(), position2);
}

}
