#pragma once

#include "gmock/gmock.h"
#include "graphics/TextureId.h"
#include "interfaces/graphics/GraphicsSystem.h"

namespace solitaire::graphics {

class GraphicsSystemMock: public interfaces::GraphicsSystem {
public:
    MOCK_METHOD(void, createWindow, (const std::string&,
                const unsigned, const unsigned), (override));

    MOCK_METHOD(TextureId, loadTexture, (const std::string&), (override));
    MOCK_METHOD(void, setTextureAlpha, (const TextureId, const uint8_t), (const, override));
    MOCK_METHOD(void, renderTexture, (const TextureId, const geometry::Position&,
                const geometry::Area&), (const, override));
    MOCK_METHOD(void, renderTextureInFullWindow, (const TextureId), (const, override));
    MOCK_METHOD(void, renderFrame, (), (const, override));
};

}
