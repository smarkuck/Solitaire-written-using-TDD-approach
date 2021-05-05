#include "events/DefaultEventsProcessor.h"
#include "events/EventsDefinitions.h"
#include "events/EventsSource.h"

namespace solitaire::events {

DefaultEventsProcessor::DefaultEventsProcessor(
    Solitaire& solitaire, std::unique_ptr<EventsSource> eventsSource):
        solitaire {solitaire},
        eventsSource {std::move(eventsSource)} {
}

void DefaultEventsProcessor::processEvents() {
    auto event = eventsSource->getEvent();
    while (not std::holds_alternative<NoEvents>(event)) {
        if (std::holds_alternative<Quit>(event)) {
            receivedQuitEvent = true;
            return;
        }

        event = eventsSource->getEvent();
    }
}

bool DefaultEventsProcessor::shouldQuit() const {
    return receivedQuitEvent;
}

}
