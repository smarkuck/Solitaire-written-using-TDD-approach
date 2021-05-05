#pragma once

#include <memory>
#include "EventsProcessor.h"

namespace solitaire {

class Solitaire;

namespace events {

class EventsSource;

class DefaultEventsProcessor: public EventsProcessor {
public:
    DefaultEventsProcessor(Solitaire&, std::unique_ptr<EventsSource>);

    void processEvents() override;
    bool shouldQuit() const override;

private:
    Solitaire& solitaire;
    std::unique_ptr<EventsSource> eventsSource;
    bool receivedQuitEvent {false};
};

}
}
