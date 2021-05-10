#pragma once

#include "geometry/Position.h"
#include "geometry/Size.h"
#include "interfaces/Button.h"

namespace solitaire {

class Button: public interfaces::Button {
public:
    Button(const geometry::Position&, const geometry::Size&);
    void setHoveredState(bool) override;
    bool collidesWith(const geometry::Position&) const override;
    bool isHovered() const override;

private:
    geometry::Position position;
    geometry::Size size;
    bool hoverState {false};
};

}
