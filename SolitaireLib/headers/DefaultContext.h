#pragma once

#include <memory>

#include "Context.h"
#include "geometry/Position.h"

namespace solitaire {

class DefaultContext: public Context {
public:
    DefaultContext(std::unique_ptr<Solitaire>);

    void setMousePosition(const geometry::Position&) override;
    void setCardsInHandPosition(const geometry::Position&) override;

    Solitaire& getSolitaire() override;
    const Solitaire& getSolitaire() const override;

    geometry::Position getMousePosition() const override;
    geometry::Position getCardsInHandPosition() const override;

private:
    std::unique_ptr<Solitaire> solitaire;
    geometry::Position mousePosition;
    geometry::Position cardsInHandPosition;
};

}
