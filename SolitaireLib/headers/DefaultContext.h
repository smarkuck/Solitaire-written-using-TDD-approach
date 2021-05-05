#pragma once

#include "Context.h"
#include "geometry/Position.h"

namespace solitaire {

class DefaultContext: public Context {
public:
    void setMousePosition(const geometry::Position&) override;
    void setCardsInHandPosition(const geometry::Position&) override;

    geometry::Position getMousePosition() const override;
    geometry::Position getCardsInHandPosition() const override;

private:
    geometry::Position mousePosition;
    geometry::Position cardsInHandPosition;
};

}
