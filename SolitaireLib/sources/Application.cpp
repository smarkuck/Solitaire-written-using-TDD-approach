#include "Application.h"
#include "Solitaire.h"
#include "events/EventsSource.h"
#include "events/EventsDefinitions.h"
#include "graphics/Renderer.h"
#include "time/FPSLimiter.h"

using namespace solitaire::events;
using namespace solitaire::graphics;
using namespace solitaire::time;

namespace solitaire {

namespace {
constexpr double frameTime {1000 / 60.0};
}

Application::Application(std::unique_ptr<Solitaire> solitaire,
                         std::unique_ptr<EventsSource> eventsSource,
                         std::unique_ptr<Renderer> renderer,
                         std::unique_ptr<FPSLimiter> fpsLimiter):
    solitaire {std::move(solitaire)},
    eventsSource {std::move(eventsSource)},
    renderer {std::move(renderer)},
    fpsLimiter {std::move(fpsLimiter)} {
}

void Application::run() const {
    solitaire->startNewGame();

    while (true) {
        fpsLimiter->saveFrameStartTime();

        auto event = eventsSource->getEvent();
        while (not std::holds_alternative<NoEvents>(event)) {
            if (std::holds_alternative<Quit>(event))
                return;

            event = eventsSource->getEvent();
        }

        renderer->render();
        fpsLimiter->sleepRestOfFrameTime();
    }
}

}
