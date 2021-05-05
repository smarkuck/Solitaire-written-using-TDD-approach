#pragma once

#include <memory>
#include "EventsProcessor.h"

namespace solitaire {

class Context;
class Solitaire;

namespace events {

class EventsSource;

class DefaultEventsProcessor: public EventsProcessor {
public:
    DefaultEventsProcessor(Solitaire&, Context&, std::unique_ptr<EventsSource>);

    void processEvents() override;
    bool shouldQuit() const override;

private:
    bool receivedQuitEvent {false};

    Solitaire& solitaire;
    Context& context;
    std::unique_ptr<EventsSource> eventsSource;

};

}
}
