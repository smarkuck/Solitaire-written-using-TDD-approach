#pragma once

#include <stdexcept>
#include <memory>

template <class T>
class mock_ptr {
public:
    ~mock_ptr() {
        if (not isUniquePointerCreated)
            delete rawPointer;
    }

    std::unique_ptr<T> make_unique() {
        if (isUniquePointerCreated)
            throw std::runtime_error {"unique_ptr has already been created"};

        isUniquePointerCreated = true;
        return std::unique_ptr<T>(rawPointer);
    }

    T& operator*() {
        return *rawPointer;
    }

private:
    T* rawPointer = new T;
    bool isUniquePointerCreated = false;
};
