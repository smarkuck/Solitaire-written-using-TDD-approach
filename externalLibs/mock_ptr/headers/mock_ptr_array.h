#pragma once

#include <algorithm>
#include <array>

#include "mock_ptr.h"

template <class T, std::size_t N>
struct mock_ptr_array: std::array<mock_ptr<T>, N> {
    template <class UniquePtrType = T>
    std::array<std::unique_ptr<UniquePtrType>, N> make_unique() {
        std::array<std::unique_ptr<UniquePtrType>, N> result;
        std::transform(this->begin(), this->end(), result.begin(), [] (auto& mockPtr) {
            return mockPtr.make_unique();
        });
        return result;
    }
};
