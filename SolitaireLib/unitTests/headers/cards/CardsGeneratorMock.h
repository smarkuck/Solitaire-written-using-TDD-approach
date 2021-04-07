#pragma once

#include "cards/CardsGenerator.h"
#include "gmock/gmock.h"

namespace solitaire {
namespace cards {

class CardsGeneratorMock: public CardsGenerator {
public:
    MOCK_METHOD(Cards, generate, (), (const, override));
};

}
}
