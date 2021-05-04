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

class Application {
public:
    Application(std::unique_ptr<Solitaire>,
                std::unique_ptr<events::EventsSource>,
                std::unique_ptr<graphics::Renderer>);

    void run() const;

private:
    std::unique_ptr<Solitaire> solitaire;
    std::unique_ptr<events::EventsSource> eventsSource;
    std::unique_ptr<graphics::Renderer> renderer;
};

}
