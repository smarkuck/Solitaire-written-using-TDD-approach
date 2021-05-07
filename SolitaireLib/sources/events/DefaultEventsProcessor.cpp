#include "Context.h"
#include "Solitaire.h"
#include "events/DefaultEventsProcessor.h"
#include "events/EventsDefinitions.h"
#include "events/EventsSource.h"
#include "interfaces/colliders/FoundationPileCollider.h"
#include "piles/PileId.h"

using namespace solitaire::geometry;
using namespace solitaire::piles;

namespace solitaire::events {

DefaultEventsProcessor::DefaultEventsProcessor(
    Context& context, std::unique_ptr<EventsSource> eventsSource):
        context {context},
        eventsSource {std::move(eventsSource)} {
}

void DefaultEventsProcessor::processEvents() {
    Event event = eventsSource->getEvent();
    while (eventOccured(event)) {
        processEvent(event);
        if (shouldQuit()) return;
        event = eventsSource->getEvent();
    }
}

bool DefaultEventsProcessor::eventOccured(const Event& event) const {
    return not std::holds_alternative<NoEvents>(event);
}

void DefaultEventsProcessor::processEvent(const Event& event) {
    if (std::holds_alternative<Quit>(event))
        receivedQuitEvent = true;
    else if (std::holds_alternative<MouseLeftButtonDown>(event))
        processMouseLeftButtonDownEvent(std::get<MouseLeftButtonDown>(event));
    else if (std::holds_alternative<MouseLeftButtonUp>(event))
        processMouseLeftButtonUpEvent();
    else if (std::holds_alternative<MouseMove>(event))
        processMouseMoveEvent(std::get<MouseMove>(event));
}

void DefaultEventsProcessor::processMouseLeftButtonDownEvent(
    const MouseLeftButtonDown& event) const
{
    for (PileId id {0}; id < Solitaire::foundationPilesCount; ++id) {
        const auto& collider = context.getFoundationPileCollider(id);
        if (collider.collidesWithPoint(event.position))
        {
            context.getSolitaire().tryPullOutCardFromFoundationPile(id);
            context.setMousePosition(event.position);
            context.setCardsInHandPosition(collider.getPosition());
            return;
        }
    }
}

void DefaultEventsProcessor::processMouseLeftButtonUpEvent() const {
    auto& solitaire = context.getSolitaire();
    tryAddCardOnFoundationPile(solitaire, context.getCardsInHandPosition());
    solitaire.tryPutCardsBackFromHand();
}

void DefaultEventsProcessor::tryAddCardOnFoundationPile(
    Solitaire& solitaire, const Position& cardsInHandPosition) const
{
    for (PileId id {0}; id < Solitaire::foundationPilesCount; ++id) {
        const auto& collider = context.getFoundationPileCollider(id);
        if (collider.collidesWithCard(cardsInHandPosition))
        {
            solitaire.tryAddCardOnFoundationPile(id);
            break;
        }
    }
}

void DefaultEventsProcessor::processMouseMoveEvent(const MouseMove& event) const {
    const auto lastMousePosition = context.getMousePosition();
    auto lastCardsInHandPosition = context.getCardsInHandPosition();
    auto mouseMoveDelta = event.position - lastMousePosition;
    context.setMousePosition(event.position);
    context.setCardsInHandPosition(lastCardsInHandPosition + mouseMoveDelta);
}

bool DefaultEventsProcessor::shouldQuit() const {
    return receivedQuitEvent;
}

}
