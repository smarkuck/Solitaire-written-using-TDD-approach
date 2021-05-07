#pragma once

namespace solitaire::graphics::interfaces {

class Renderer {
public:
    virtual ~Renderer() = default;
    virtual void render() const = 0;
};

}
