#pragma once

#include <memory>

#include "Event.h"
#include "interfaces/events/EventsProcessor.h"

namespace solitaire::interfaces {
class Solitaire;
}

namespace solitaire::colliders::interfaces {
class FoundationPileCollider;
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

    void processEvent(const Event& event);
    void processMouseLeftButtonDownEvent(const MouseLeftButtonDown&) const;
    void processMouseLeftButtonUpEvent() const;
    void processMouseMoveEvent(const MouseMove&) const;

    bool checkIfCollidesAndTryPullOutCardFromAnyFoundationPile(
        const MouseLeftButtonDown&) const;
    bool checkIfCollidesAndTryPullOutCardFromFoundationPile(
        const piles::PileId, const MouseLeftButtonDown&) const;
    void tryPullOutCardFromFoundationPile(
        const piles::PileId, const MouseLeftButtonDown&,
        const colliders::interfaces::FoundationPileCollider&) const;

    void tryPullOutOrUncoverCardsFromAnyTableauPile(
        const MouseLeftButtonDown&) const;
    bool checkIfCollidesAndTryPullOutOrUncoverCardsFromTableauPile(
        const piles::PileId, const MouseLeftButtonDown&) const;
    void tryPullOutOrUncoverCardsFromTableauPile(
        const piles::PileId, const MouseLeftButtonDown&,
        const unsigned cardIndex) const;
    void tryPullOutCardsFromTableauPile(
        const piles::PileId, const MouseLeftButtonDown&,
        const TableauPileMouseLeftButtonDownEventData&) const;

    void tryAddCardOnFoundationPile(solitaire::interfaces::Solitaire&,
        const geometry::Position& cardsInHandPosition) const;

    TableauPileMouseLeftButtonDownEventData
    getTableauPileMouseLeftButtonDownEventData(
        const piles::PileId, const unsigned cardIndex) const;

    bool shouldTryUncoverTableauPileCard(
        const TableauPileMouseLeftButtonDownEventData&) const;

    bool eventOccured(const Event& event) const;
    bool receivedQuitEvent {false};

    solitaire::interfaces::Context& context;
    std::unique_ptr<interfaces::EventsSource> eventsSource;
};

}
