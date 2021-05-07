#pragma once

#include "gmock/gmock.h"
#include "interfaces/graphics/Renderer.h"

namespace solitaire::graphics {

class RendererMock: public interfaces::Renderer {
public:
    MOCK_METHOD(void, render, (), (const, override));
};

}
