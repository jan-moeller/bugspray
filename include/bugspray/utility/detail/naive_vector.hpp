//
// MIT License
//
// Copyright (c) 2022 Jan Möller
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#ifndef BUGSPRAY_NAIVE_VECTOR_HPP
#define BUGSPRAY_NAIVE_VECTOR_HPP

#include <algorithm>
#include <concepts>
#include <initializer_list>
#include <iterator>
#include <memory>
#include <utility>

#include <cassert>
#include <cstdint>

/*
 * Implements a subset of the functionality of std::vector. This is meant as a stopgap solution while many stdlibs don't
 * yet fully support constexpr vector.
 */

namespace bs
{
template<typename T>
class naive_vector
{
  public:
    using value_type      = T;
    using reference       = T&;
    using const_reference = T const&;
    using pointer         = T*;
    using const_pointer   = T const*;
    using iterator        = T*;
    using const_iterator  = T const*;
    using difference_type = std::ptrdiff_t;

    constexpr naive_vector() = default;
    constexpr ~naive_vector() noexcept
    {
        if (capacity() > 0)
        {
            std::destroy_n(m_begin, size());
            m_allocator.deallocate(m_begin, capacity());
        }
    }

    constexpr naive_vector(std::initializer_list<T> ilist)
        : naive_vector(ilist.begin(), ilist.end())
    {
    }

    template<std::forward_iterator Begin, std::sentinel_for<Begin> End>
    constexpr naive_vector(Begin begin, End end)
    {
        for (std::forward_iterator auto iter = begin; iter != end; ++iter)
            push_back(*iter);
    }

    constexpr naive_vector(naive_vector const& other)
        : m_begin(other.capacity() > 0 ? m_allocator.allocate(other.capacity()) : nullptr)
        , m_end(m_begin ? m_begin + other.size() : nullptr)
        , m_capacity_end(m_begin ? m_begin + other.capacity() : nullptr)
    {
        for (std::size_t i = 0; i < other.size(); ++i)
            std::construct_at(m_begin + i, other[i]);
    }

    constexpr naive_vector(naive_vector&& other) noexcept
        : m_begin(std::exchange(other.m_begin, nullptr))
        , m_end(std::exchange(other.m_end, nullptr))
        , m_capacity_end(std::exchange(other.m_capacity_end, nullptr))
    {
    }

    constexpr auto operator=(naive_vector const& other) -> naive_vector&
    {
        if (this != &other)
        {
            if (capacity() > 0)
            {
                std::destroy_n(m_begin, size());
                m_allocator.deallocate(m_begin, capacity());
            }
            m_begin        = other.capacity() > 0 ? m_allocator.allocate(other.capacity()) : nullptr;
            m_end          = m_begin ? m_begin + other.size() : nullptr;
            m_capacity_end = m_begin ? m_begin + other.capacity() : nullptr;
            for (std::size_t i = 0; i < other.size(); ++i)
                std::construct_at(m_begin + i, other[i]);
        }
        return *this;
    }

    constexpr auto operator=(naive_vector&& other) -> naive_vector&
    {
        if (this != &other)
        {
            if (capacity() > 0)
            {
                std::destroy_n(m_begin, size());
                m_allocator.deallocate(m_begin, capacity());
            }
            m_begin        = std::exchange(other.m_begin, nullptr);
            m_end          = std::exchange(other.m_end, nullptr);
            m_capacity_end = std::exchange(other.m_capacity_end, nullptr);
        }
        return *this;
    }

    [[nodiscard]] constexpr auto front() const noexcept -> T const& { return *begin(); }
    [[nodiscard]] constexpr auto front() noexcept -> T& { return *begin(); }
    [[nodiscard]] constexpr auto back() const noexcept -> T const& { return *(end() - 1); }
    [[nodiscard]] constexpr auto back() noexcept -> T& { return *(end() - 1); }

    [[nodiscard]] constexpr auto begin() const noexcept -> T const* { return m_begin; }
    [[nodiscard]] constexpr auto begin() noexcept -> T* { return m_begin; }

    [[nodiscard]] constexpr auto end() const noexcept -> T const* { return m_end; }
    [[nodiscard]] constexpr auto end() noexcept -> T* { return m_end; }

    [[nodiscard]] constexpr auto size() const noexcept -> std::size_t { return m_begin ? m_end - m_begin : 0; }
    [[nodiscard]] constexpr auto capacity() const noexcept -> std::size_t
    {
        return m_begin ? m_capacity_end - m_begin : 0;
    }
    [[nodiscard]] constexpr auto empty() const noexcept -> bool { return size() == 0; }

    constexpr void push_back(T const& value) { emplace_back(value); }

    template<typename... Ts>
    constexpr auto emplace_back(Ts&&... values) -> reference
    {
        if (m_capacity_end == m_end)
        {
            auto const s         = size();
            auto const c         = capacity();
            auto const new_c     = c > 0 ? 2 * c : 1;
            T*         new_begin = m_allocator.allocate(new_c);
            for (std::size_t i = 0; i < s; ++i)
                std::construct_at(new_begin + i, (*this)[i]);
            if (c > 0)
            {
                std::destroy_n(m_begin, s);
                m_allocator.deallocate(m_begin, c);
            }
            m_begin        = new_begin;
            m_end          = m_begin + s;
            m_capacity_end = m_begin + new_c;
        }
        std::construct_at(m_end, T{std::forward<Ts>(values)...});
        return *(m_end++);
    }

    constexpr void pop()
    {
        assert(!empty());
        std::destroy_at(&back());
        --m_end;
    }

    constexpr auto operator[](std::size_t idx) const noexcept -> const_reference { return *(begin() + idx); }
    constexpr auto operator[](std::size_t idx) noexcept -> reference { return *(begin() + idx); }

    friend constexpr auto operator==(naive_vector<T> const& lhs, naive_vector<T> const& rhs) noexcept -> bool
    {
        return std::ranges::equal(lhs, rhs);
    }

  private:
    std::allocator<T> m_allocator{};

    T* m_begin        = nullptr;
    T* m_end          = nullptr;
    T* m_capacity_end = nullptr;

    friend class naive_string;
};
} // namespace bs

#endif // BUGSPRAY_NAIVE_VECTOR_HPP
