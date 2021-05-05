#pragma once

namespace solitaire::events {

class EventsProcessor {
public:
    virtual ~EventsProcessor() = default;
    virtual void processEvents() = 0;
    virtual bool shouldQuit() const = 0;
};

}
