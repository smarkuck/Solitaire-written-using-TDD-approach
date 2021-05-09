#include <iostream>

#include "Application.h"
#include "ApplicationFactory.h"
#include "SDL.h"
#include "interfaces/Context.h"
#include "interfaces/events/EventsProcessor.h"
#include "interfaces/graphics/Renderer.h"
#include "interfaces/time/FPSLimiter.h"

int main(int, char**) try {
    ApplicationFactory {}.make().run();
    return 0;
}
catch (const std::runtime_error& e) {
    std::cout << e.what() << std::endl;
    return -1;
}
