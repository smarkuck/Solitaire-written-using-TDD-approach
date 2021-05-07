#pragma once

#include <memory>
#include <string>

namespace solitaire {
class Application;
}

namespace solitaire::events::interfaces {
class EventsProcessor;
}

namespace solitaire::graphics::interfaces {
class Renderer;
}

namespace solitaire::interfaces {
class Context;
class Solitaire;
}

namespace solitaire::time::interfaces {
class FPSLimiter;
}

class ApplicationFactory {
public:
    solitaire::Application make() const;

private:
    std::unique_ptr<solitaire::interfaces::Context> makeContext() const;
    std::unique_ptr<solitaire::interfaces::Solitaire> makeSolitaire() const;

    std::unique_ptr<solitaire::events::interfaces::EventsProcessor>
    makeEventsProcessor(solitaire::interfaces::Context&) const;

    std::unique_ptr<solitaire::graphics::interfaces::Renderer>
    makeRenderer(const solitaire::interfaces::Context&) const;

    std::unique_ptr<solitaire::time::interfaces::FPSLimiter>
    makeFPSLimiter() const;

    std::string findAssetsPath() const;
};
