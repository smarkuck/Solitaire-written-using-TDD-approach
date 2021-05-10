#include "Button.h"
#include "cards/Card.h"
#include "events/EventsDefinitions.h"
#include "events/EventsProcessor.h"
#include "interfaces/Context.h"
#include "interfaces/Solitaire.h"
#include "interfaces/colliders/FoundationPileCollider.h"
#include "interfaces/colliders/StockPileCollider.h"
#include "interfaces/colliders/TableauPileCollider.h"
#include "interfaces/events/EventsSource.h"
#include "piles/PileId.h"
#include "piles/TableauPile.h"

using namespace solitaire::cards;
using namespace solitaire::colliders::interfaces;
using namespace solitaire::events::interfaces;
using namespace solitaire::geometry;
using namespace solitaire::interfaces;
using namespace solitaire::piles;

namespace solitaire::events {

EventsProcessor::EventsProcessor(
    Context& context, std::unique_ptr<EventsSource> eventsSource):
        context {context},
        eventsSource {std::move(eventsSource)} {
}

void EventsProcessor::processEvents() {
    Event event = eventsSource->getEvent();
    while (eventOccured(event)) {
        processEvent(event);
        if (shouldQuit()) return;
        event = eventsSource->getEvent();
    }
}

bool EventsProcessor::eventOccured(const Event& event) const {
    return not std::holds_alternative<NoEvents>(event);
}

void EventsProcessor::processEvent(const Event& event) {
    if (std::holds_alternative<Quit>(event))
        receivedQuitEvent = true;
    else if (std::holds_alternative<MouseLeftButtonDown>(event))
        processMouseLeftButtonDownEvent(std::get<MouseLeftButtonDown>(event));
    else if (std::holds_alternative<MouseLeftButtonUp>(event))
        processMouseLeftButtonUpEvent();
    else if (std::holds_alternative<MouseMove>(event))
        processMouseMoveEvent(std::get<MouseMove>(event));
}

void EventsProcessor::processMouseLeftButtonDownEvent(
    const MouseLeftButtonDown& event) const
{
    if (checkIfCollidesAndTryClickButtons(event)) return;
    if (checkIfCollidesAndTryPullOutCardFromAnyFoundationPile(event)) return;
    if (checkIfCollidesAndTryPullOutOrUncoverCardsFromAnyTableauPile(event)) return;
    tryInteractWithStockPile(event);
}

bool EventsProcessor::checkIfCollidesAndTryClickButtons(
    const MouseLeftButtonDown& event) const
{
    if (context.getNewGameButton().collidesWith(event.position)) {
        context.getSolitaire().startNewGame();
        return true;
    }
    return false;
}

bool EventsProcessor::
checkIfCollidesAndTryPullOutCardFromAnyFoundationPile(
    const MouseLeftButtonDown& event) const
{
    for (PileId id {0}; id < Solitaire::foundationPilesCount; ++id)
        if (checkIfCollidesAndTryPullOutCardFromFoundationPile(id, event))
            return true;
    return false;
}

bool EventsProcessor::checkIfCollidesAndTryPullOutCardFromFoundationPile(
    const PileId id, const MouseLeftButtonDown& event) const
{
    const auto& collider = context.getFoundationPileCollider(id);
    if (collider.collidesWithPoint(event.position))
    {
        tryPullOutCardFromFoundationPile(id, event, collider);
        return true;
    }
    return false;
}

void EventsProcessor::tryPullOutCardFromFoundationPile(
    const PileId id, const MouseLeftButtonDown& event,
    const FoundationPileCollider& collider) const
{
    context.getSolitaire().tryPullOutCardFromFoundationPile(id);
    context.setMousePosition(event.position);
    context.setCardsInHandPosition(collider.getPosition());
}

bool EventsProcessor::
checkIfCollidesAndTryPullOutOrUncoverCardsFromAnyTableauPile(
    const MouseLeftButtonDown& event) const
{
    for (PileId id {0}; id < Solitaire::tableauPilesCount; ++id)
        if (checkIfCollidesAndTryPullOutOrUncoverCardsFromTableauPile(id, event))
            return true;
    return false;
}

bool EventsProcessor::checkIfCollidesAndTryPullOutOrUncoverCardsFromTableauPile(
    const PileId id, const MouseLeftButtonDown& event) const
{
    const auto& collider = context.getTableauPileCollider(id);
    const auto cardIndex = collider.tryGetCollidedCardIndex(event.position);
    if (cardIndex) {
        tryPullOutOrUncoverCardsFromTableauPile(id, event, cardIndex.value());
        return true;
    }
    return false;
}

void EventsProcessor::tryPullOutOrUncoverCardsFromTableauPile(
    const PileId id, const MouseLeftButtonDown& event,
    const unsigned cardIndex) const
{
    const auto eventData = getTableauPileMouseLeftButtonDownEventData(id, cardIndex);
    if (shouldTryUncoverTableauPileCard(eventData))
        eventData.solitaire.tryUncoverTableauPileTopCard(id);
    else
        tryPullOutCardsFromTableauPile(id, event, eventData);
}

EventsProcessor::TableauPileMouseLeftButtonDownEventData
EventsProcessor::getTableauPileMouseLeftButtonDownEventData(
    const PileId id, const unsigned cardIndex) const
{
    auto& solitaire = context.getSolitaire();
    auto& tableauPile = solitaire.getTableauPile(id);

    return TableauPileMouseLeftButtonDownEventData {
        solitaire,
        tableauPile,
        context.getTableauPileCollider(id),
        static_cast<unsigned>(tableauPile.getCards().size()),
        cardIndex
    };
}

bool EventsProcessor::shouldTryUncoverTableauPileCard(
    const TableauPileMouseLeftButtonDownEventData& eventData) const
{
    return eventData.cardIndex == eventData.cardsQuantity - 1 and
           eventData.pile.isTopCardCovered();
}

void EventsProcessor::tryPullOutCardsFromTableauPile(
    const PileId id, const MouseLeftButtonDown& event,
    const TableauPileMouseLeftButtonDownEventData& eventData) const
{
    context.setMousePosition(event.position);
    context.setCardsInHandPosition(
        eventData.collider.getCardPosition(eventData.cardIndex));
    const auto cardsToPullOut = eventData.cardsQuantity - eventData.cardIndex;
    eventData.solitaire.tryPullOutCardsFromTableauPile(id, cardsToPullOut);
}

void EventsProcessor::tryInteractWithStockPile(const MouseLeftButtonDown& event) const {
    const auto& collider = context.getStockPileCollider();
    if (collider.coveredCardsCollidesWith(event.position))
        context.getSolitaire().trySelectNextStockPileCard();
    else if (collider.uncoveredCardsCollidesWith(event.position))
        tryPullOutCardFromStockPile(event, collider);
}

void EventsProcessor::tryPullOutCardFromStockPile(
    const MouseLeftButtonDown& event, const StockPileCollider& collider) const
{
    context.setMousePosition(event.position);
    context.setCardsInHandPosition(collider.getUncoveredCardsPosition());
    context.getSolitaire().tryPullOutCardFromStockPile();
}

void EventsProcessor::processMouseLeftButtonUpEvent() const {
    const auto eventData = getMouseLeftButtonUpEventData();
    if (eventData.cardsInHand.empty()) return;
    if (not tryAddCardOnAnyFoundationPileAndCheckIfHandEmpty(eventData))
        tryAddCardsOnAnyTableauPile(eventData);
    eventData.solitaire.tryPutCardsBackFromHand();
}

EventsProcessor::MouseLeftButtonUpEventData
EventsProcessor::getMouseLeftButtonUpEventData() const {
    auto& solitaire = context.getSolitaire();
    return MouseLeftButtonUpEventData {
        solitaire,
        solitaire.getCardsInHand(),
        context.getCardsInHandPosition()
    };
}

bool EventsProcessor::tryAddCardOnAnyFoundationPileAndCheckIfHandEmpty(
    const MouseLeftButtonUpEventData& eventData) const
{
    for (PileId id {0}; id < Solitaire::foundationPilesCount; ++id)
        if (tryAddCardOnFoundationPileIfCollidesAndCheckIfHandEmpty(id, eventData))
            return true;
    return false;
}

bool EventsProcessor::tryAddCardOnFoundationPileIfCollidesAndCheckIfHandEmpty(
    const PileId id, const MouseLeftButtonUpEventData& eventData) const
{
    const auto& collider = context.getFoundationPileCollider(id);
    return collider.collidesWithCardsInHand(eventData.cardsInHandPosition) and
           tryAddCardOnFoundationPileAndCheckIfHandEmpty(id, eventData);
}

bool EventsProcessor::tryAddCardOnFoundationPileAndCheckIfHandEmpty(
    const PileId id, const MouseLeftButtonUpEventData& eventData) const
{
    eventData.solitaire.tryAddCardOnFoundationPile(id);
    return eventData.cardsInHand.empty();
}

void EventsProcessor::tryAddCardsOnAnyTableauPile(
    const MouseLeftButtonUpEventData& eventData) const
{
    for (PileId id {0}; id < Solitaire::tableauPilesCount; ++id) {
        const auto& collider = context.getTableauPileCollider(id);
        if (collider.collidesWithCardsInHand(eventData.cardsInHandPosition) and
            tryAddCardOnTableauPileAndCheckIfHandEmpty(id, eventData))
            return;
    }
}

bool EventsProcessor::tryAddCardOnTableauPileAndCheckIfHandEmpty(
    const PileId id, const MouseLeftButtonUpEventData& eventData) const
{
    eventData.solitaire.tryAddCardsOnTableauPile(id);
    return eventData.cardsInHand.empty();
}

void EventsProcessor::processMouseMoveEvent(const MouseMove& event) const {
    processButtonsHoverState(event);
    const auto lastMousePosition = context.getMousePosition();
    auto lastCardsInHandPosition = context.getCardsInHandPosition();
    auto mouseMoveDelta = event.position - lastMousePosition;
    context.setMousePosition(event.position);
    context.setCardsInHandPosition(lastCardsInHandPosition + mouseMoveDelta);
}

void EventsProcessor::processButtonsHoverState(const MouseMove& event) const {
    auto& newGameButton = context.getNewGameButton();
    newGameButton.setHoveredState(
        newGameButton.collidesWith(event.position));
}

bool EventsProcessor::shouldQuit() const {
    return receivedQuitEvent;
}

}
