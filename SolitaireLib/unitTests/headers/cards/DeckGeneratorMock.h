#pragma once

#include "gmock/gmock.h"
#include "interfaces/cards/DeckGenerator.h"

namespace solitaire::cards {

class DeckGeneratorMock: public interfaces::DeckGenerator {
public:
    MOCK_METHOD(Deck, generate, (), (const, override));
};

}
