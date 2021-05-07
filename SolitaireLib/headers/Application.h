#pragma once

#include <memory>

namespace solitaire {

namespace interfaces {
class Context;
}

namespace events::interfaces {
class EventsProcessor;
}

namespace graphics::interfaces {
class Renderer;
}

namespace time::interfaces {
class FPSLimiter;
}

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
