#pragma once

namespace solitaire::geometry {
struct Position;
}

namespace solitaire::interfaces {

class Button {
public:
    virtual ~Button() = default;
    virtual void setHoveredState(bool) = 0;
    virtual bool collidesWith(const geometry::Position&) const = 0;
    virtual bool isHovered() const = 0;
};

}
