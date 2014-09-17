/// \file
///
/// This file is written by [Kenny Kerr](http://kennykerr.ca) and based on MSDN article
/// [Windows and C++](http://msdn.microsoft.com/en-us/magazine/hh288076.aspx).
///
/// This file was belong to [dx project by Kenny Kerr](http://dx.codeplex.com/),
/// but was slightly modified to unbind from platform-dependent headers
/// and support on linux-family OS.
#ifndef __HANDLE_HPP__
#define __HANDLE_HPP__

#include "platform.h"

#ifndef ASSERT
#include <cassert>
#define ASSERT(x) assert(x)
#endif

#ifndef VERIFY
#define VERIFY(x)
#endif

namespace KennyKerr
{
    /// \brief RAII class for storing platform-dependent handles in type-safe manner.
    ///
    /// This class is useful with
    ///
    /// - native Win32 handles
    /// - COM-interfaces (see \ref get_address_of())
    /// - other stuff like FILE handles in C
    ///   (need to implement custom traits, but code will be cleaner)
    ///
    /// For typical usage see MSDN article
    /// [Windows and C++](http://msdn.microsoft.com/en-us/magazine/hh288076.aspx)
    /// and Visual Studio Magazine article
    /// [Resource Management in the Windows API](http://visualstudiomagazine.com/articles/2013/09/01/get-a-handle-on-the-windows-api.aspx).
    template <typename Traits>
    class unique_handle
    {
        using pointer = typename Traits::pointer;

        pointer m_value;

        auto close() throw() -> void
        {
            if (*this)
            {
                Traits::close(m_value);
            }
        }

    public:

        unique_handle(unique_handle const &) = delete;
        auto operator=(unique_handle const &) -> unique_handle & = delete;

        explicit unique_handle(pointer value = Traits::invalid()) throw() :
            m_value { value }
        {
        }

        unique_handle(unique_handle && other) throw() :
            m_value { other.release() }
        {
        }

        auto operator=(unique_handle && other) throw() -> unique_handle &
        {
            if (this != &other)
            {
                reset(other.release());
            }

            return *this;
        }

        ~unique_handle() throw()
        {
            close();
        }

        explicit operator bool() const throw()
        {
            return m_value != Traits::invalid();
        }

        auto get() const throw() -> pointer
        {
            return m_value;
        }

        auto get_address_of() throw() -> pointer *
        {
            ASSERT(!*this);
            return &m_value;
        }

        auto release() throw() -> pointer
        {
            auto value = m_value;
            m_value = Traits::invalid();
            return value;
        }

        auto reset(pointer value = Traits::invalid()) throw() -> bool
        {
            if (m_value != value)
            {
                close();
                m_value = value;
            }

            return static_cast<bool>(*this);
        }

        auto swap(unique_handle<Traits> & other) throw() -> void
        {
            std::swap(m_value, other.m_value);
        }
    };

    template <typename Traits>
    auto swap(unique_handle<Traits> & left,
              unique_handle<Traits> & right) throw() -> void
    {
        left.swap(right);
    }

    template <typename Traits>
    auto operator==(unique_handle<Traits> const & left,
                    unique_handle<Traits> const & right) throw() -> bool
    {
        return left.get() == right.get();
    }

    template <typename Traits>
    auto operator!=(unique_handle<Traits> const & left,
                    unique_handle<Traits> const & right) throw() -> bool
    {
        return left.get() != right.get();
    }

    template <typename Traits>
    auto operator<(unique_handle<Traits> const & left,
                   unique_handle<Traits> const & right) throw() -> bool
    {
        return left.get() < right.get();
    }

    template <typename Traits>
    auto operator>=(unique_handle<Traits> const & left,
                    unique_handle<Traits> const & right) throw() -> bool
    {
        return left.get() >= right.get();
    }

    template <typename Traits>
    auto operator>(unique_handle<Traits> const & left,
                   unique_handle<Traits> const & right) throw() -> bool
    {
        return left.get() > right.get();
    }

    template <typename Traits>
    auto operator<=(unique_handle<Traits> const & left,
                    unique_handle<Traits> const & right) throw() -> bool
    {
        return left.get() <= right.get();
    }

#ifdef PLATFORM_WIN32

    struct null_handle_traits
    {
        using pointer = HANDLE;

        static auto invalid() throw() -> pointer
        {
            return nullptr;
        }

        static auto close(pointer value) throw() -> void
        {
            VERIFY(CloseHandle(value));
        }
    };

    struct invalid_handle_traits
    {
        using pointer = HANDLE;

        static auto invalid() throw() -> pointer
        {
            return INVALID_HANDLE_VALUE;
        }

        static auto close(pointer value) throw() -> void
        {
            VERIFY(CloseHandle(value));
        }
    };

    using null_handle = unique_handle<null_handle_traits>;
    using invalid_handle = unique_handle<invalid_handle_traits>;

#endif

}

#endif