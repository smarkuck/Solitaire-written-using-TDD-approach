#pragma once

namespace solitaire {

namespace graphics {
struct TexturePosition;
}

class Context {
public:
    virtual ~Context() = default;

    virtual void setMousePosition(const graphics::TexturePosition&) = 0;
    virtual void setCardsInHandPosition(const graphics::TexturePosition&) = 0;

    virtual graphics::TexturePosition getMousePosition() const = 0;
    virtual graphics::TexturePosition getCardsInHandPosition() const = 0;
};

}
