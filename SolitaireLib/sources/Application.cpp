#include "Application.h"
#include "interfaces/Context.h"
#include "interfaces/Solitaire.h"
#include "interfaces/events/EventsProcessor.h"
#include "interfaces/graphics/Renderer.h"
#include "interfaces/time/FPSLimiter.h"

using namespace solitaire::events::interfaces;
using namespace solitaire::graphics::interfaces;
using namespace solitaire::interfaces;
using namespace solitaire::time::interfaces;

namespace solitaire {

Application::Application(std::unique_ptr<Context> context,
                         std::unique_ptr<EventsProcessor> eventsProcessor,
                         std::unique_ptr<Renderer> renderer,
                         std::unique_ptr<FPSLimiter> fpsLimiter):
    context {std::move(context)},
    eventsProcessor {std::move(eventsProcessor)},
    renderer {std::move(renderer)},
    fpsLimiter {std::move(fpsLimiter)} {
}

void Application::run() const {
    context->getSolitaire().startNewGame();

    while (not eventsProcessor->shouldQuit()) {
        fpsLimiter->saveFrameStartTime();
        eventsProcessor->processEvents();
        renderer->render();
        fpsLimiter->sleepRestOfFrameTime();
    }
}

}
