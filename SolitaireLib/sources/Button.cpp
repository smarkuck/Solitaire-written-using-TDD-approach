#include "Button.h"
#include "Layout.h"

namespace solitaire {

Button::Button(const geometry::Position& position, const geometry::Size& size):
    position {position}, size {size} {
}

void Button::setHoveredState(bool state) {
    hoverState = state;
}

bool Button::collidesWith(const geometry::Position& position) const {
    return position.x >= this->position.x and
           position.x < this->position.x + size.width and
           position.y >= this->position.y and
           position.y < this->position.y + size.height;
}

bool Button::isHovered() const {
    return hoverState;
}

}
