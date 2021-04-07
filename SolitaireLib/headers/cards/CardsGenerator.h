#pragma once

#include "Cards.h"

namespace solitaire {
namespace cards {

class CardsGenerator {
public:
    virtual ~CardsGenerator() = default;
    virtual Cards generate() const = 0;
};

}
}
