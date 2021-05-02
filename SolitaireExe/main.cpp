#include <cassert>
#include <filesystem>

#include "DefaultSolitaire.h"
#include "archivers/DefaultHistoryTracker.h"
#include "archivers/DefaultMoveCardsOperationSnapshotCreator.h"
#include "cards/ShuffledDeckGenerator.h"
#include "cards/Value.h"
#include "cards/Suit.h"
#include "graphics/DefaultSDLWrapper.h"
#include "graphics/Renderer.h"
#include "graphics/SDLGraphicsSystem.h"
#include "piles/FoundationPile.h"
#include "piles/StockPile.h"
#include "piles/TableauPile.h"

using namespace solitaire;

std::string findAssetsPath() {
    std::string path = "assets/";
    const std::string moveUp = "../";

    for (unsigned i = 0; i < 4; ++i, path = moveUp + path)
        if (std::filesystem::exists(path))
            return path;

    assert(false and "cannot find assets path");
    return {};
}

class FoundationPileStub: public piles::FoundationPile {
public:
    void initialize() override {}
    void tryAddCard(std::optional<cards::Card>& cardToAdd) override {}
    std::optional<cards::Card> tryPullOutCard() override { return std::nullopt; }

    const cards::Cards& getCards() const override {
        return cards;
    }

    std::optional<cards::Value> getTopCardValue() const override { return std::nullopt; }
    std::unique_ptr<archivers::Snapshot> createSnapshot() override { return nullptr; }

private:
    cards::Cards cards {
        cards::Card {cards::Value::Ace, cards::Suit::Heart},
        cards::Card {cards::Value::Seven, cards::Suit::Diamond}
    };
};

class TableauPileStub: public piles::TableauPile {
public:
    void initialize(const cards::Deck::const_iterator& begin,
                    const cards::Deck::const_iterator& end) override {};

    void tryUncoverTopCard() override {};
    void tryAddCards(cards::Cards& cardsToAdd) override {};
    cards::Cards tryPullOutCards(unsigned quantity) override { return {}; };

    const cards::Cards& getCards() const override { return cards; };
    unsigned getTopCoveredCardPosition() const override { return 1; };
    bool isTopCardCovered() const override { return true; };
    std::unique_ptr<archivers::Snapshot> createSnapshot() override { return nullptr; }

private:
    cards::Cards cards {
        cards::Card {cards::Value::Ace, cards::Suit::Heart},
        cards::Card {cards::Value::Seven, cards::Suit::Spade},
        cards::Card {cards::Value::King, cards::Suit::Club},
    };
};


class StockPileStub: public piles::StockPile {
public:
    void initialize(const cards::Deck::const_iterator& begin,
                    const cards::Deck::const_iterator& end) override {};

    void trySelectNextCard() override {};

    std::optional<cards::Card> tryPullOutCard() override {
        return cards::Card {cards::Value::Ace, cards::Suit::Heart};
    };

    const cards::Cards& getCards() const override { return cards; };
    std::optional<unsigned> getSelectedCardIndex() const override { return 7; };
    std::unique_ptr<archivers::Snapshot> createSnapshot() override { return nullptr; }

private:
    cards::Cards cards {
        cards::Card {cards::Value::Ace, cards::Suit::Heart},
        cards::Card {cards::Value::Seven, cards::Suit::Spade},
        cards::Card {cards::Value::King, cards::Suit::Club},
        cards::Card {cards::Value::Ace, cards::Suit::Heart},
        cards::Card {cards::Value::Seven, cards::Suit::Spade},
        cards::Card {cards::Value::King, cards::Suit::Club},

        cards::Card {cards::Value::Ace, cards::Suit::Heart},
        cards::Card {cards::Value::Seven, cards::Suit::Spade},
        cards::Card {cards::Value::King, cards::Suit::Club},
        cards::Card {cards::Value::Ace, cards::Suit::Heart},
        cards::Card {cards::Value::Seven, cards::Suit::Spade},
        cards::Card {cards::Value::King, cards::Suit::Club},

        cards::Card {cards::Value::King, cards::Suit::Club},
    };
};

int main(int, char**) {
    Solitaire::FoundationPiles foundationPiles;
    for (auto& pile: foundationPiles)
        pile = std::make_shared<FoundationPileStub>();

    Solitaire::TableauPiles tableauPiles;
    for (auto& pile: tableauPiles)
        pile = std::make_shared<TableauPileStub>();

    DefaultSolitaire solitaire {
        std::make_unique<cards::ShuffledDeckGenerator>(),
        std::make_shared<StockPileStub>(),
        foundationPiles, tableauPiles,
        std::make_unique<archivers::DefaultHistoryTracker>(),
        std::make_unique<archivers::DefaultMoveCardsOperationSnapshotCreator>()
    };

    solitaire.startNewGame();

    graphics::Renderer renderer {
        solitaire,
        std::make_unique<graphics::SDLGraphicsSystem>(
            std::make_unique<graphics::DefaultSDLWrapper>()
        ),
        findAssetsPath()
    };

    for (unsigned i = 0; i < 500; ++i)
        renderer.render();

    return 0;
}
