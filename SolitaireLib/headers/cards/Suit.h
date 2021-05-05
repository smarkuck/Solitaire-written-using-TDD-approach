#pragma once

#include <string>

namespace solitaire::cards {

enum class Suit {
    Heart, Spade, Diamond, Club
};

inline int to_int(const Suit& suit) {
    return static_cast<int>(suit);
}

inline std::string to_string(const Suit& suit) {
    switch(suit) {
        case Suit::Heart:
            return "Heart";
        case Suit::Spade:
            return "Spade";
        case Suit::Diamond:
            return "Diamond";
        case Suit::Club:
            return "Club";
        default:
            return "Unknown";
    }
}

}
