#pragma once

#include <string>

namespace solitaire {

class GraphicsSystem;
class Solitaire;

namespace graphics {

class SDLWrapper;

class Renderer {
public:
    Renderer(const Solitaire&, GraphicsSystem&);

    void render() const;

private:
    static std::string findAssetsPath();

    static const std::string assetsPath;

    const Solitaire& solitaire;
    GraphicsSystem& system;
};

}
}
