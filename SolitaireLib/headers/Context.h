#pragma once

namespace solitaire {

class Solitaire;

namespace geometry {
struct Position;
}

class Context {
public:
    virtual ~Context() = default;

    virtual void setMousePosition(const geometry::Position&) = 0;
    virtual void setCardsInHandPosition(const geometry::Position&) = 0;

    virtual Solitaire& getSolitaire() = 0;
    virtual const Solitaire& getSolitaire() const = 0;

    virtual geometry::Position getMousePosition() const = 0;
    virtual geometry::Position getCardsInHandPosition() const = 0;
};

}
