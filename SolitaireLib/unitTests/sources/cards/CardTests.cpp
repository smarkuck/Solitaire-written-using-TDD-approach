#include "cards/Card.h"
#include "cards/Suit.h"
#include "cards/Value.h"
#include "gtest/gtest.h"

using namespace testing;

namespace solitaire::cards {

TEST(CardTest, hasValueOneGreaterThanOtherCard) {
    const Suit heart = Suit::Heart;
    EXPECT_FALSE(Card (Value::King, heart).hasValueOneGreaterThan(Card {Value::King, heart}));
    EXPECT_TRUE(Card (Value::King, heart).hasValueOneGreaterThan(Card {Value::Queen, heart}));
    EXPECT_FALSE(Card (Value::King, heart).hasValueOneGreaterThan(Card {Value::Jack, heart}));
    EXPECT_FALSE(Card (Value::Queen, heart).hasValueOneGreaterThan(Card {Value::King, heart}));
}

TEST(CardTest, hasSameSuit) {
    const Value king = Value::King;
    EXPECT_TRUE(Card (king, Suit::Heart).hasSameSuitAs(Card {king, Suit::Heart}));
    EXPECT_TRUE(Card (king, Suit::Spade).hasSameSuitAs(Card {king, Suit::Spade}));
    EXPECT_FALSE(Card (king, Suit::Heart).hasSameSuitAs(Card {king, Suit::Spade}));
    EXPECT_FALSE(Card (king, Suit::Spade).hasSameSuitAs(Card {king, Suit::Heart}));
}

TEST(CardTest, hasRedColor) {
    const Value king = Value::King;
    EXPECT_TRUE(Card (king, Suit::Heart).hasRedColor());
    EXPECT_FALSE(Card (king, Suit::Spade).hasRedColor());
    EXPECT_TRUE(Card (king, Suit::Diamond).hasRedColor());
    EXPECT_FALSE(Card (king, Suit::Club).hasRedColor());
}

TEST(CardTest, hasDifferentColorThanOtherCard) {
    const Value king = Value::King;
    EXPECT_FALSE(Card (king, Suit::Heart).hasDifferentColorThan(Card {king, Suit::Heart}));
    EXPECT_FALSE(Card (king, Suit::Spade).hasDifferentColorThan(Card {king, Suit::Spade}));
    EXPECT_TRUE(Card (king, Suit::Heart).hasDifferentColorThan(Card {king, Suit::Spade}));
    EXPECT_TRUE(Card (king, Suit::Spade).hasDifferentColorThan(Card {king, Suit::Heart}));
}

}
