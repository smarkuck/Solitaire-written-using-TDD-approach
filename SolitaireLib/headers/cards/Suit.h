#pragma once

#include <string>

namespace solitaire::cards {

enum class Suit {
    Heart, Spade, Diamond, Club
};

inline std::string to_string(const Suit& color) {
    switch(color) {
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
