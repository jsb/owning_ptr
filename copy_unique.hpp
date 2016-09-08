#pragma once

#include <memory>

template<typename T>
std::unique_ptr<T> copy_unique(const std::unique_ptr<T>& _source)
{
    return _source ? std::make_unique<T>(*_source) : nullptr;
}
