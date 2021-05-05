#include "Application.h"
#include "ApplicationFactory.h"
#include "Context.h"
#include "SDL.h"
#include "Solitaire.h"
#include "events/EventsProcessor.h"
#include "graphics/Renderer.h"
#include "time/FPSLimiter.h"

int main(int, char**) {
    ApplicationFactory {}.make().run();
    return 0;
}
