#pragma once

#include "copy_unique.hpp"

#include <memory>

template<typename T>
class owning_ptr final
{
public:
    using pointer = T*;
    using element_type = T;

    constexpr owning_ptr() noexcept
    {
    }

    constexpr owning_ptr(std::nullptr_t) noexcept
    {
    }

    explicit owning_ptr(pointer _p) noexcept :
        ptr_(_p)
    {
    }

    owning_ptr(const owning_ptr& _rhs) noexcept :
        ptr_(copy_unique(_rhs.ptr_))
    {
    }

    owning_ptr(owning_ptr&& _rhs) noexcept :
        ptr_(std::move(_rhs.ptr_))
    {
    }

    owning_ptr& operator=(const owning_ptr& _rhs) noexcept
    {
        ptr_ = copy_unique(_rhs.ptr_);
        return *this;
    }

    owning_ptr& operator=(owning_ptr&& _rhs) noexcept
    {
        ptr_ = std::move(_rhs.ptr_);
        return *this;
    }

    owning_ptr& operator=(std::nullptr_t) noexcept
    {
        ptr_ = nullptr;
        return *this;
    }

    pointer release() noexcept
    {
        return ptr_.release();
    }

    void reset(pointer _p = pointer()) noexcept
    {
        ptr_.reset(_p);
    }

    void reset(std::nullptr_t) noexcept
    {
        ptr_.reset(nullptr);
    }

    void swap(owning_ptr& _other) noexcept
    {
        ptr_.swap(_other);
    }

    pointer get() const noexcept
    {
        return ptr_.get();
    }

    explicit operator bool() const noexcept
    {
        return static_cast<bool>(ptr_);
    }

    typename std::add_lvalue_reference<element_type>::type operator*() const noexcept
    {
        return *ptr_;
    }

    pointer operator->() const noexcept
    {
        return ptr_.get();
    }

private:
    std::unique_ptr<T> ptr_;
};

template<typename T, class... Args>
owning_ptr<T> make_owning(Args&&... _args)
{
    return owning_ptr<T>(new T(std::forward<Args>(_args)...));
}

template<typename T>
void swap(owning_ptr<T>& _lhs, owning_ptr<T>& _rhs) noexcept
{
    _lhs.swap(_rhs);
}

namespace std
{
    template<typename T>
    struct hash<owning_ptr<T>>
    {
        using argument_type = owning_ptr<T>;
        using result_type = std::size_t;
        result_type operator()(const argument_type& _s) const noexcept
        {
            return std::hash<typename owning_ptr<T>::pointer>()(_s.get());
        }
    };
}

template<typename T1, typename T2>
bool operator==(const owning_ptr<T1>& _x, const owning_ptr<T2>& _y) noexcept
{
    return _x.get() == _y.get();
}

template<typename T1>
bool operator==(const owning_ptr<T1>& _x, std::nullptr_t) noexcept
{
    return !_x;
}

template<typename T2>
bool operator==(std::nullptr_t, const owning_ptr<T2>& _y) noexcept
{
    return !_y;
}

template<typename T1, typename T2>
bool operator!=(const owning_ptr<T1>& _x, const owning_ptr<T2>& _y) noexcept
{
    return _x.get() != _y.get();
}

template<typename T1>
bool operator!=(const owning_ptr<T1>& _x, std::nullptr_t) noexcept
{
    return _x;
}

template<typename T2>
bool operator!=(std::nullptr_t, const owning_ptr<T2>& _y) noexcept
{
    return _y;
}

template<typename T1, typename T2>
bool operator<(const owning_ptr<T1>& _x, const owning_ptr<T2>& _y) noexcept
{
    using ct = typename std::common_type<typename owning_ptr<T1>::pointer, typename owning_ptr<T2>::pointer>::type;
    return std::less<ct>(_x.get(), _y.get());
}

template<typename T1>
bool operator<(const owning_ptr<T1>& _x, std::nullptr_t) noexcept
{
    return std::less<typename owning_ptr<T1>::pointer>(_x.get(), nullptr);
}

template<typename T2>
bool operator<(std::nullptr_t, const owning_ptr<T2>& _y) noexcept
{
    return std::less<typename owning_ptr<T2>::pointer>(nullptr, _y.get());
}

template<typename T1, typename T2>
bool operator>(const owning_ptr<T1>& _x, const owning_ptr<T2>& _y) noexcept
{
    return _y < _x;
}

template<typename T1>
bool operator>(const owning_ptr<T1>& _x, std::nullptr_t) noexcept
{
    return nullptr < _x;
}

template<typename T2>
bool operator>(std::nullptr_t, const owning_ptr<T2>& _y) noexcept
{
    return _y < nullptr;
}

template<typename T1, typename T2>
bool operator<=(const owning_ptr<T1>& _x, const owning_ptr<T2>& _y) noexcept
{
    return !(_y < _x);
}

template<typename T1>
bool operator<=(const owning_ptr<T1>& _x, std::nullptr_t) noexcept
{
    return !(nullptr < _x);
}

template<typename T2>
bool operator<=(std::nullptr_t, const owning_ptr<T2>& _y) noexcept
{
    return !(_y < nullptr);
}

template<typename T1, typename T2>
bool operator>=(const owning_ptr<T1>& _x, const owning_ptr<T2>& _y) noexcept
{
    return !(_x < _y);
}

template<typename T1>
bool operator>=(const owning_ptr<T1>& _x, std::nullptr_t) noexcept
{
    return !(_x < nullptr);
}

template<typename T2>
bool operator>=(std::nullptr_t, const owning_ptr<T2>& _y) noexcept
{
    return !(nullptr < _y);
}
