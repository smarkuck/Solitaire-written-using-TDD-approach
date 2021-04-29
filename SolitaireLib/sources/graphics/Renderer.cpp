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

Renderer::Renderer(const Solitaire& solitaire, GraphicsSystem& system):
    solitaire {solitaire},
    system {system}
{
    system.createWindow("Solitaire", 640, 480);
    system.loadTexture(assetsPath + "background.bmp");
    system.loadTexture(assetsPath + "cards.bmp");
    system.loadTexture(assetsPath + "card_placeholder.bmp");
    system.setTextureAlpha(TextureId{2}, 70);
}

void Renderer::render() const {
    system.renderTextureOnFullscreen(TextureId{0});

    TextureArea area {
        TexturePosition {0, 0},
        TextureSize {75, 104}
    };

    TexturePosition position {0, 0};

    for (unsigned i = 0; i < 8; ++i) {
        position.x = i*80;
        area.position.x = i*75;
        system.renderTexture(TextureId{1}, position, area);
    }

    position.y = 109;
    area.position.x = 0;
    area.position.y = 416;
    for (unsigned i = 0; i < 4; ++i) {
        position.x = i*80;
        system.renderTexture(TextureId{1}, position, area);
    }

    position.x = 0;
    position.y = 218;
    area.position.x = 0;
    area.position.y = 0;
    system.renderTexture(TextureId{2}, position, area);

    system.renderFrame();
}

}
