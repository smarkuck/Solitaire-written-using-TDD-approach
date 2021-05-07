#pragma once

#include <memory>

namespace solitaire::events::interfaces {
class EventsProcessor;
}

namespace solitaire::graphics::interfaces {
class Renderer;
}

namespace solitaire::interfaces {
class Context;
}

namespace solitaire::time::interfaces {
class FPSLimiter;
}

namespace solitaire {

class Application {
public:
    Application(std::unique_ptr<interfaces::Context>,
                std::unique_ptr<events::interfaces::EventsProcessor>,
                std::unique_ptr<graphics::interfaces::Renderer>,
                std::unique_ptr<time::interfaces::FPSLimiter>);

    void run() const;

private:
    std::unique_ptr<interfaces::Context> context;
    std::unique_ptr<events::interfaces::EventsProcessor> eventsProcessor;
    std::unique_ptr<graphics::interfaces::Renderer> renderer;
    std::unique_ptr<time::interfaces::FPSLimiter> fpsLimiter;
};

}
