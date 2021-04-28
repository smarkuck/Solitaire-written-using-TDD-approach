#pragma once

#include <functional>
#include <memory>

namespace solitaire::graphics {

template <class T>
using SDLPtr = std::unique_ptr<T, std::function<void (T*)>>;

}
