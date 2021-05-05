#pragma once

namespace solitaire {

namespace geometry {
struct Position;
}

class Context {
public:
    virtual ~Context() = default;

    virtual void setMousePosition(const geometry::Position&) = 0;
    virtual void setCardsInHandPosition(const geometry::Position&) = 0;

    virtual geometry::Position getMousePosition() const = 0;
    virtual geometry::Position getCardsInHandPosition() const = 0;
};

}
