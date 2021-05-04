#include "Application.h"
#include "Solitaire.h"
#include "events/EventsSource.h"
#include "events/EventsDefinitions.h"
#include "graphics/Renderer.h"

using namespace solitaire::events;
using namespace solitaire::graphics;

namespace solitaire {

Application::Application(std::unique_ptr<Solitaire> solitaire,
                         std::unique_ptr<EventsSource> eventsSource,
                         std::unique_ptr<Renderer> renderer):
    solitaire {std::move(solitaire)},
    eventsSource {std::move(eventsSource)},
    renderer {std::move(renderer)} {
}

void Application::run() const {
    auto event = eventsSource->getEvent();

    while (not std::holds_alternative<Quit>(event)) {
        renderer->render();
        event = eventsSource->getEvent();
    }
}

}
