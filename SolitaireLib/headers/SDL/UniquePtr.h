#pragma once

#include <functional>
#include <memory>

namespace solitaire::SDL {

template <class T>
using UniquePtr = std::unique_ptr<T, std::function<void (T*)>>;

}
