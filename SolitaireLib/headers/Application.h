#pragma once

#include <memory>

namespace solitaire {

class Solitaire;

namespace events {
class EventsSource;
}

namespace graphics {
class Renderer;
}

namespace time {
class FPSLimiter;
}

class Application {
public:
    Application(std::unique_ptr<Solitaire>,
                std::unique_ptr<events::EventsSource>,
                std::unique_ptr<graphics::Renderer>,
                std::unique_ptr<time::FPSLimiter>);

    void run() const;

private:
    std::unique_ptr<Solitaire> solitaire;
    std::unique_ptr<events::EventsSource> eventsSource;
    std::unique_ptr<graphics::Renderer> renderer;
    std::unique_ptr<time::FPSLimiter> fpsLimiter;
};

}
