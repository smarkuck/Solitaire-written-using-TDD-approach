#include <cmath>

#include "Context.h"
#include "Solitaire.h"
#include "events/DefaultEventsProcessor.h"
#include "events/EventsDefinitions.h"
#include "events/EventsSource.h"
#include "geometry/Layout.h"
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
    auto event = eventsSource->getEvent();
    while (not std::holds_alternative<NoEvents>(event)) {
        if (std::holds_alternative<Quit>(event)) {
            receivedQuitEvent = true;
            return;
        }

        if (std::holds_alternative<MouseLeftButtonDown>(event)) {
            const auto& mouseLeftButtonDown = std::get<MouseLeftButtonDown>(event);
            for (PileId id {0}; id < Solitaire::foundationPilesCount; ++id) {
                const int pileX = Layout::firstFoundationPilePositionX + Layout::pilesSpacing * id;
                if (mouseLeftButtonDown.position.x >= pileX and
                    mouseLeftButtonDown.position.x < pileX + Layout::cardSize.width and
                    mouseLeftButtonDown.position.y >= Layout::foundationPilePositionY and
                    mouseLeftButtonDown.position.y < Layout::foundationPilePositionY + Layout::cardSize.height)
                {
                    context.getSolitaire().tryPullOutCardFromFoundationPile(id);
                    context.setMousePosition(mouseLeftButtonDown.position);
                    context.setCardsInHandPosition(Position {pileX, Layout::foundationPilePositionY});
                    break;
                }
            }
        }
        else if (std::holds_alternative<MouseMove>(event)) {
            const auto& mouseMove = std::get<MouseMove>(event);
            const auto lastMousePosition = context.getMousePosition();
            auto lastCardsInHandPosition = context.getCardsInHandPosition();
            auto diff = mouseMove.position - lastMousePosition;
            context.setMousePosition(mouseMove.position);
            context.setCardsInHandPosition(lastCardsInHandPosition + diff);
        }
        else if (std::holds_alternative<MouseLeftButtonUp>(event)) {
            const auto cardsInHandPosition = context.getCardsInHandPosition();

            for (PileId id {0}; id < Solitaire::foundationPilesCount; ++id) {
                const int pileX = Layout::firstFoundationPilePositionX + Layout::pilesSpacing * id;
                if (std::abs(cardsInHandPosition.x - pileX) < Layout::cardSize.width and
                    std::abs(cardsInHandPosition.y - Layout::foundationPilePositionY) < Layout::cardSize.height)
                {
                    context.getSolitaire().tryAddCardOnFoundationPile(id);
                    break;
                }
            }

            context.getSolitaire().tryPutCardsBackFromHand();
        }

        event = eventsSource->getEvent();
    }
}

bool DefaultEventsProcessor::shouldQuit() const {
    return receivedQuitEvent;
}

}
