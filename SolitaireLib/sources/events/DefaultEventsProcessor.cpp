#include <cmath>

#include "Context.h"
#include "Solitaire.h"
#include "events/DefaultEventsProcessor.h"
#include "events/EventsDefinitions.h"
#include "events/EventsSource.h"
#include "graphics/Layout.h"
#include "piles/PileId.h"

using namespace solitaire::graphics;
using namespace solitaire::piles;

namespace solitaire::events {

DefaultEventsProcessor::DefaultEventsProcessor(
    Solitaire& solitaire, Context& context, std::unique_ptr<EventsSource> eventsSource):
        solitaire {solitaire},
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
                if (mouseLeftButtonDown.x >= pileX and
                    mouseLeftButtonDown.x < pileX + Layout::cardSize.width and
                    mouseLeftButtonDown.y >= Layout::foundationPilePositionY and
                    mouseLeftButtonDown.y < Layout::foundationPilePositionY + Layout::cardSize.height)
                {
                    solitaire.tryPullOutCardFromFoundationPile(id);
                    context.setMousePosition(TexturePosition {mouseLeftButtonDown.x, mouseLeftButtonDown.y});
                    context.setCardsInHandPosition(TexturePosition {pileX, Layout::foundationPilePositionY});
                    break;
                }
            }
        }
        else if (std::holds_alternative<MouseMove>(event)) {
            const auto& mouseMove = std::get<MouseMove>(event);
            const auto lastMousePosition = context.getMousePosition();
            auto lastCardsInHandPosition = context.getCardsInHandPosition();
            int diffX = mouseMove.x - lastMousePosition.x;
            int diffY = mouseMove.y - lastMousePosition.y;
            context.setMousePosition(TexturePosition {mouseMove.x, mouseMove.y});
            context.setCardsInHandPosition(TexturePosition {
                lastCardsInHandPosition.x + diffX, lastCardsInHandPosition.y + diffY});
        }
        else if (std::holds_alternative<MouseLeftButtonUp>(event)) {
            const auto cardsInHandPosition = context.getCardsInHandPosition();

            for (PileId id {0}; id < Solitaire::foundationPilesCount; ++id) {
                const int pileX = Layout::firstFoundationPilePositionX + Layout::pilesSpacing * id;
                if (std::abs(cardsInHandPosition.x - pileX) < Layout::cardSize.width and
                    std::abs(cardsInHandPosition.y - Layout::foundationPilePositionY) < Layout::cardSize.height)
                {
                    solitaire.tryAddCardOnFoundationPile(id);
                    break;
                }
            }

            solitaire.tryPutCardsBackFromHand();
        }

        event = eventsSource->getEvent();
    }
}

bool DefaultEventsProcessor::shouldQuit() const {
    return receivedQuitEvent;
}

}
