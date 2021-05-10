#pragma once

#include "gmock/gmock.h"
#include "interfaces/Button.h"

namespace solitaire {

class ButtonMock: public interfaces::Button {
public:
    MOCK_METHOD(void, setHoveredState, (bool state), (override));
    MOCK_METHOD(bool, collidesWith, (const geometry::Position&), (const, override));
    MOCK_METHOD(bool, isHovered, (), (const, override));
};

}
