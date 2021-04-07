#pragma once

#include "cards/CardsGenerator.h"

namespace solitaire {
namespace cards {

class CardsGeneratorMock: public CardsGenerator {
public:
    MOCK_METHOD(Cards, generate, (), (const, override));
};

}
}
