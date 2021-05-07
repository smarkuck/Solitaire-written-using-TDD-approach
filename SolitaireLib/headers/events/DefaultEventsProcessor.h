#pragma once

#include <memory>

#include "Event.h"
#include "interfaces/events/EventsProcessor.h"

namespace solitaire {

class Context;

namespace events {

namespace interfaces {
class EventsSource;
}

class DefaultEventsProcessor: public interfaces::EventsProcessor {
public:
    DefaultEventsProcessor(Context&, std::unique_ptr<interfaces::EventsSource>);

    void processEvents() override;
    bool shouldQuit() const override;

private:
    void processEvent(const Event& event);
    void processMouseLeftButtonDownEvent(const MouseLeftButtonDown&) const;
    void processMouseLeftButtonUpEvent() const;
    void processMouseMoveEvent(const MouseMove&) const;

    void tryAddCardOnFoundationPile(Solitaire&,
        const geometry::Position& cardsInHandPosition) const;

    bool eventOccured(const Event& event) const;
    bool receivedQuitEvent {false};

    Context& context;
    std::unique_ptr<interfaces::EventsSource> eventsSource;
};

}
}
