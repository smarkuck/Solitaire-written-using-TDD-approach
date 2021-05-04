#pragma once

#include <memory>
#include <string>

namespace solitaire {
class Application;
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
}

class ApplicationFactory {
public:
    solitaire::Application make() const;

private:
    std::unique_ptr<solitaire::Solitaire> makeSolitaire() const;
    std::unique_ptr<solitaire::events::EventsSource> makeEventsSource() const;
    std::unique_ptr<solitaire::graphics::Renderer> makeRenderer(
        const solitaire::Solitaire&) const;
    std::unique_ptr<solitaire::time::FPSLimiter> makeFPSLimiter() const;

    std::string findAssetsPath() const;
};