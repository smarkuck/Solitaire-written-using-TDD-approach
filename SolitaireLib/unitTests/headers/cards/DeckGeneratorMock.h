#pragma once

#include "cards/DeckGenerator.h"
#include "gmock/gmock.h"

namespace solitaire::cards {

class DeckGeneratorMock: public DeckGenerator {
public:
    MOCK_METHOD(Deck, generate, (), (const, override));
};

}
