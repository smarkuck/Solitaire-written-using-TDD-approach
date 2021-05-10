#pragma once

#include <memory>

#include "Event.h"
#include "cards/Cards.h"
#include "interfaces/events/EventsProcessor.h"

namespace solitaire::interfaces {
class Solitaire;
}

namespace solitaire::colliders::interfaces {
class FoundationPileCollider;
class StockPileCollider;
class TableauPileCollider;
}

namespace solitaire::events::interfaces {
class EventsSource;
}

namespace solitaire::geometry {
struct Position;
}

namespace solitaire::interfaces {
class Context;
class Solitaire;
}

namespace solitaire::piles {
struct PileId;
}

namespace solitaire::piles::interfaces {
class TableauPile;
}

namespace solitaire::events {

class EventsProcessor: public interfaces::EventsProcessor {
public:
    EventsProcessor(solitaire::interfaces::Context&,
                    std::unique_ptr<interfaces::EventsSource>);

    void processEvents() override;
    bool shouldQuit() const override;

private:
    struct TableauPileMouseLeftButtonDownEventData {
        solitaire::interfaces::Solitaire& solitaire;
        const piles::interfaces::TableauPile& pile;
        colliders::interfaces::TableauPileCollider& collider;
        unsigned cardsQuantity;
        unsigned cardIndex;
    };

    struct MouseLeftButtonUpEventData {
        solitaire::interfaces::Solitaire& solitaire;
        const cards::Cards& cardsInHand;
        geometry::Position cardsInHandPosition;
    };

    void processEvent(const Event& event);
    void processMouseLeftButtonDownEvent(const MouseLeftButtonDown&) const;
    void processMouseLeftButtonUpEvent() const;
    void processMouseMoveEvent(const MouseMove&) const;
    void processButtonsHoverState(const MouseMove&) const;

    bool checkIfCollidesAndTryClickButtons(const MouseLeftButtonDown&) const;
    bool checkIfCollidesAndTryPullOutCardFromAnyFoundationPile(
        const MouseLeftButtonDown&) const;
    bool checkIfCollidesAndTryPullOutCardFromFoundationPile(
        const piles::PileId, const MouseLeftButtonDown&) const;
    void tryPullOutCardFromFoundationPile(
        const piles::PileId, const MouseLeftButtonDown&,
        const colliders::interfaces::FoundationPileCollider&) const;

    bool checkIfCollidesAndTryPullOutOrUncoverCardsFromAnyTableauPile(
        const MouseLeftButtonDown&) const;
    bool checkIfCollidesAndTryPullOutOrUncoverCardsFromTableauPile(
        const piles::PileId, const MouseLeftButtonDown&) const;
    void tryPullOutOrUncoverCardsFromTableauPile(
        const piles::PileId, const MouseLeftButtonDown&,
        const unsigned cardIndex) const;
    void tryPullOutCardsFromTableauPile(
        const piles::PileId, const MouseLeftButtonDown&,
        const TableauPileMouseLeftButtonDownEventData&) const;

    bool tryAddCardOnAnyFoundationPileAndCheckIfHandEmpty(
        const MouseLeftButtonUpEventData&) const;
    bool tryAddCardOnFoundationPileIfCollidesAndCheckIfHandEmpty(
        const piles::PileId, const MouseLeftButtonUpEventData&) const;
    bool tryAddCardOnFoundationPileAndCheckIfHandEmpty(
        const piles::PileId, const MouseLeftButtonUpEventData&) const;
    void tryAddCardsOnAnyTableauPile(const MouseLeftButtonUpEventData&) const;
    bool tryAddCardOnTableauPileAndCheckIfHandEmpty(
        const piles::PileId, const MouseLeftButtonUpEventData&) const;

    void tryInteractWithStockPile(const MouseLeftButtonDown&) const;
    void tryPullOutCardFromStockPile(
        const MouseLeftButtonDown&,
        const colliders::interfaces::StockPileCollider&) const;

    TableauPileMouseLeftButtonDownEventData
    getTableauPileMouseLeftButtonDownEventData(
        const piles::PileId, const unsigned cardIndex) const;

    MouseLeftButtonUpEventData getMouseLeftButtonUpEventData() const;

    bool shouldTryUncoverTableauPileCard(
        const TableauPileMouseLeftButtonDownEventData&) const;

    bool eventOccured(const Event& event) const;
    bool receivedQuitEvent {false};

    solitaire::interfaces::Context& context;
    std::unique_ptr<interfaces::EventsSource> eventsSource;
};

}
