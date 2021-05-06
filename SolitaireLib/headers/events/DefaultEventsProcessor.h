#pragma once

#include <memory>
#include "EventsProcessor.h"

namespace solitaire {

class Context;

namespace events {

class EventsSource;

class DefaultEventsProcessor: public EventsProcessor {
public:
    DefaultEventsProcessor(Context&, std::unique_ptr<EventsSource>);

    void processEvents() override;
    bool shouldQuit() const override;

private:
    bool receivedQuitEvent {false};

    Context& context;
    std::unique_ptr<EventsSource> eventsSource;
};

}
}
