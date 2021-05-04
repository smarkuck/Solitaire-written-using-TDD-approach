#pragma once

#include <variant>

namespace solitaire::events {

struct NoEvents;
struct MouseLeftButtonDown;
struct MouseLeftButtonUp;
struct MouseMove;
struct Quit;

using Event = std::variant<NoEvents, MouseLeftButtonDown, MouseLeftButtonUp, MouseMove, Quit>;

}
