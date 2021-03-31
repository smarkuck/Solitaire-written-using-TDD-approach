#pragma once

#include <string>

namespace solitaire {
namespace cards {

enum class Value {
    Ace, Two, Three, Four, Five,
    Six, Seven, Eight, Nine, Ten,
    Jack, Queen, King
};

inline std::string to_string(const Value& value) {
    switch(value) {
        case Value::Ace:
            return "Ace";
        case Value::Two:
            return "Two";
        case Value::Three:
            return "Three";
        case Value::Four:
            return "Four";
        case Value::Five:
            return "Five";
        case Value::Six:
            return "Six";
        case Value::Seven:
            return "Seven";
        case Value::Eight:
            return "Eight";
        case Value::Nine:
            return "Nine";
        case Value::Ten:
            return "Ten";
        case Value::Jack:
            return "Jack";
        case Value::Queen:
            return "Queen";
        case Value::King:
            return "King";
        default:
            return "Unknown";
    }
}

}
}
