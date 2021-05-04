#pragma once

#include "gmock/gmock.h"
#include "graphics/Renderer.h"

namespace solitaire::graphics {

class RendererMock: public Renderer {
public:
    MOCK_METHOD(void, render, (), (const, override));
};

}
