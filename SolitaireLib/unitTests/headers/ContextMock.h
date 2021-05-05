#pragma once

#include "Context.h"
#include "geometry/Position.h"
#include "gmock/gmock.h"

namespace solitaire {

class ContextMock: public Context {
public:
    MOCK_METHOD(void, setMousePosition, (const geometry::Position&), (override));
    MOCK_METHOD(void, setCardsInHandPosition, (const geometry::Position&), (override));

    MOCK_METHOD(geometry::Position, getMousePosition, (), (const, override));
    MOCK_METHOD(geometry::Position, getCardsInHandPosition, (), (const, override));
};

}
