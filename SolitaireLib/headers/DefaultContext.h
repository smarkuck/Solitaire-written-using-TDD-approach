#pragma once

#include "Context.h"
#include "graphics/TexturePosition.h"

namespace solitaire {

class DefaultContext: public Context {
public:
    void setMousePosition(const graphics::TexturePosition&) override;
    void setCardsInHandPosition(const graphics::TexturePosition&) override;

    graphics::TexturePosition getMousePosition() const override;
    graphics::TexturePosition getCardsInHandPosition() const override;

private:
    graphics::TexturePosition mousePosition;
    graphics::TexturePosition cardsInHandPosition;
};

}
