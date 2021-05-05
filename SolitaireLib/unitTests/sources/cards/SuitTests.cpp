#include "cards/Suit.h"
#include "gtest/gtest.h"

using namespace testing;

namespace solitaire::cards {

TEST(SuitTests, to_int) {
    EXPECT_EQ(to_int(Suit::Heart), 0);
    EXPECT_EQ(to_int(Suit::Spade), 1);
    EXPECT_EQ(to_int(Suit::Diamond), 2);
    EXPECT_EQ(to_int(Suit::Club), 3);
}

TEST(SuitTests, to_string) {
    EXPECT_EQ(to_string(Suit::Heart), "Heart");
    EXPECT_EQ(to_string(Suit::Spade), "Spade");
    EXPECT_EQ(to_string(Suit::Diamond), "Diamond");
    EXPECT_EQ(to_string(Suit::Club), "Club");
    EXPECT_EQ(to_string(Suit {10}), "Unknown");
}

}
