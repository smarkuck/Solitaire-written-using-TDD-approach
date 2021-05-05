#include "events/EventsDefinitions.h"
#include "gtest/gtest.h"

using namespace testing;
using namespace solitaire::geometry;

namespace solitaire::events {

TEST(EventsDefinitionsTests, equalOperator) {
    Position position1 {3, 8};
    Position position2 {4, 7};

    EXPECT_TRUE(NoEvents {} == NoEvents {});
    EXPECT_TRUE(MouseLeftButtonUp {} == MouseLeftButtonUp {});
    EXPECT_TRUE(Quit {} == Quit {});

    EXPECT_TRUE(MouseLeftButtonDown {position1} == MouseLeftButtonDown {position1});
    EXPECT_FALSE(MouseLeftButtonDown {position1} == MouseLeftButtonDown {position2});

    EXPECT_TRUE(MouseMove {position1} == MouseMove {position1});
    EXPECT_FALSE(MouseMove {position1} == MouseMove {position2});
}

}
