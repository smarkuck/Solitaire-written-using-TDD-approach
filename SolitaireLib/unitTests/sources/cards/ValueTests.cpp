#include "cards/Value.h"
#include "gtest/gtest.h"

using namespace testing;

namespace solitaire::cards {

TEST(ValueTests, to_int) {
    EXPECT_EQ(to_int(Value::Ace), 0);
    EXPECT_EQ(to_int(Value::Two), 1);
    EXPECT_EQ(to_int(Value::Three), 2);
    EXPECT_EQ(to_int(Value::Four), 3);
    EXPECT_EQ(to_int(Value::Five), 4);
    EXPECT_EQ(to_int(Value::Six), 5);
    EXPECT_EQ(to_int(Value::Seven), 6);
    EXPECT_EQ(to_int(Value::Eight), 7);
    EXPECT_EQ(to_int(Value::Nine), 8);
    EXPECT_EQ(to_int(Value::Ten), 9);
    EXPECT_EQ(to_int(Value::Jack), 10);
    EXPECT_EQ(to_int(Value::Queen), 11);
    EXPECT_EQ(to_int(Value::King), 12);
}

TEST(ValueTests, to_string) {
    EXPECT_EQ(to_string(Value::Ace), "Ace");
    EXPECT_EQ(to_string(Value::Two), "Two");
    EXPECT_EQ(to_string(Value::Three), "Three");
    EXPECT_EQ(to_string(Value::Four), "Four");
    EXPECT_EQ(to_string(Value::Five), "Five");
    EXPECT_EQ(to_string(Value::Six), "Six");
    EXPECT_EQ(to_string(Value::Seven), "Seven");
    EXPECT_EQ(to_string(Value::Eight), "Eight");
    EXPECT_EQ(to_string(Value::Nine), "Nine");
    EXPECT_EQ(to_string(Value::Ten), "Ten");
    EXPECT_EQ(to_string(Value::Jack), "Jack");
    EXPECT_EQ(to_string(Value::Queen), "Queen");
    EXPECT_EQ(to_string(Value::King), "King");
    EXPECT_EQ(to_string(Value {100}), "Unknown");
}

}
