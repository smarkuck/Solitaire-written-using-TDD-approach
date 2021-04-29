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
    system.loadTexture(assetsPath + "cards/placeholder.bmp");
    system.setTextureAlpha(TextureId{1}, 70);
    system.loadTexture(assetsPath + "cards/back.bmp");

    for (unsigned i = 0; i < 8; ++i)
        system.loadTexture(assetsPath + "cards/" + std::to_string(i) + ".bmp");
}

void Renderer::render() const {
    system.renderTextureOnFullscreen(TextureId{0});

    const TextureArea area {
        TexturePosition {0, 0},
        TextureSize {75, 104}
    };

    TexturePosition position {0, 0};

    for (unsigned i = 0; i < 8; ++i) {
        position.x = i*75;
        system.renderTexture(TextureId{i+3}, position, area);
    }

    position.y = 104;
    for (unsigned i = 0; i < 4; ++i) {
        position.x = i*75;
        system.renderTexture(TextureId{1}, position, area);
    }

    position.x = 0;
    position.y = 208;
    system.renderTexture(TextureId{2}, position, area);

    system.renderFrame();
}

}
