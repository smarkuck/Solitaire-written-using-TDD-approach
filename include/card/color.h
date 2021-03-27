#include <string>

namespace solitaire {
namespace card {

enum class Color {
    Heart, Spade, Diamond, Club
};

inline std::string to_string(const Color& color) {
    switch(color) {
        case Color::Heart:
            return "Heart";
        case Color::Spade:
            return "Spade";
        case Color::Diamond:
            return "Diamond";
        case Color::Club:
            return "Club";
        default:
            return "Unknown";
    }
}

}
}
