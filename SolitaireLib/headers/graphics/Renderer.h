#pragma once

#include <memory>
#include <string>

namespace solitaire {

class Solitaire;

namespace graphics {

class GraphicsSystem;

class Renderer {
public:
    Renderer(const Solitaire&, std::unique_ptr<GraphicsSystem>);

    void render() const;

private:
    static std::string findAssetsPath();

    static const std::string assetsPath;

    const Solitaire& solitaire;
    std::unique_ptr<GraphicsSystem> graphicsSystem;
};

}
}
