#include <cassert>
#include <filesystem>

#include "graphics/GraphicsSystem.h"
#include "graphics/Renderer.h"
#include "graphics/TextureArea.h"
#include "graphics/TextureId.h"

namespace solitaire::graphics {

const std::string Renderer::assetsPath {findAssetsPath()};

std::string Renderer::findAssetsPath() {
    std::string path = "assets/";
    const std::string moveUp = "../";

    for (unsigned i = 0; i < 4; ++i, path = moveUp + path)
        if (std::filesystem::exists(path))
            return path;

    assert(false and "cannot find assets path");
    return {};
}

Renderer::Renderer(const Solitaire& solitaire,
                   std::unique_ptr<GraphicsSystem> graphicsSystem):
    solitaire {solitaire},
    graphicsSystem {std::move(graphicsSystem)}
{
    this->graphicsSystem->createWindow("Solitaire", 640, 480);
    this->graphicsSystem->loadTexture(assetsPath + "background.bmp");
    this->graphicsSystem->loadTexture(assetsPath + "cards.bmp");
    this->graphicsSystem->loadTexture(assetsPath + "card_placeholder.bmp");
    this->graphicsSystem->setTextureAlpha(TextureId{2}, 70);
}

void Renderer::render() const {
    graphicsSystem->renderTextureOnFullscreen(TextureId{0});

    TextureArea area {
        TexturePosition {0, 0},
        TextureSize {75, 104}
    };

    TexturePosition position {0, 0};

    for (unsigned i = 0; i < 8; ++i) {
        position.x = i*80;
        area.position.x = i*75;
        graphicsSystem->renderTexture(TextureId{1}, position, area);
    }

    position.y = 109;
    area.position.x = 0;
    area.position.y = 416;
    for (unsigned i = 0; i < 4; ++i) {
        position.x = i*80;
        graphicsSystem->renderTexture(TextureId{1}, position, area);
    }

    position.x = 0;
    position.y = 218;
    area.position.x = 0;
    area.position.y = 0;
    graphicsSystem->renderTexture(TextureId{2}, position, area);

    graphicsSystem->renderFrame();
}

}
