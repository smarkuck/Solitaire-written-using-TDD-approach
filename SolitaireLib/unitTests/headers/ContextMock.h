#pragma once

#include "Context.h"
#include "gmock/gmock.h"
#include "graphics/TexturePosition.h"

namespace solitaire {

class ContextMock: public Context {
public:
    MOCK_METHOD(void, setMousePosition, (const graphics::TexturePosition&), (override));
    MOCK_METHOD(void, setCardsInHandPosition, (const graphics::TexturePosition&), (override));

    MOCK_METHOD(graphics::TexturePosition, getMousePosition, (), (const, override));
    MOCK_METHOD(graphics::TexturePosition, getCardsInHandPosition, (), (const, override));
};

}
