#pragma once

namespace solitaire::graphics {

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render() const = 0;
};

}
