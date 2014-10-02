/// \file
///
/// \ingroup platform
///
/// \brief   Timers
///
/// Initial implementation was made by Mattew Wilson in article
/// [Win32 Performance Measurement Options](http://www.drdobbs.com/windows/win32-performance-measurement-options/184416651).
/// Here is slight modified version.
///
/// On Windows implementations uses \c QueryPerformanceCounter() function.
/// On Linux implementation uses \c gettimeofday() function.
///

#ifndef __PERFOMANCE_COUNTER_H__
#define __PERFOMANCE_COUNTER_H__

#include "platform.h"

#ifndef assert
#include <cassert>
#endif

namespace platform
{

    /// \brief Timer initializer.
    ///
    /// For performance purposes, performance_counter implementation
    /// doesn't initialize internal data before first calling start()/stop().
    /// Therefore methods like performance_counter.get_seconds()
    /// will not return right data before initialization.
    ///
    /// You can use this class to have initialized performace_counter
    /// (see typedef \ref performance_initialized_timer):
    /// \code
    /// platform::performance_initialized_timer timer;
    /// \endcode
    template <typename Counter>
    class timer_initialiser : public Counter
    {
    public:
        typedef Counter counter_type;
        typedef timer_initialiser<Counter> class_type;

    public:
        /// Initialize timer by calling start()/stop()
        timer_initialiser()
        {
            counter_type::start();
            counter_type::stop();
        }
    };


    /// \brief RAII implementation for call start()/stop() methods on counter_type.
    template <typename Counter>
    class timer_scope
    {
    public:
        typedef Counter counter_type;
        typedef timer_scope<counter_type> class_type;

    public:
        explicit timer_scope(counter_type &counter) : counter_(counter)
        {
            counter_.start();
        }
        ~timer_scope() { counter_.stop(); }
        void stop() { counter_.stop(); }

        // Method has const qualifier to guarantee accessibility stop() method only
        const counter_type &get_counter() const
        {
            return counter_;
        }

    private:
        counter_type &counter_;

        timer_scope(const class_type &rhs);
        class_type &operator=(const class_type &rhs);
    };
}

#if defined PLATFORM_WIN32
#include <windows.h>
#elif defined PLATFORM_LINUX
#include <sys/time.h>
#endif

namespace platform
{
    /// \brief High-resolution timer.
    ///
    /// Implementation for Windows using \c QueryPerformanceCounter().
    /// If OS doesn't support \c QueryPerformanceCounter() interface,
    /// \c GetTickCount() function will be used
    /// (note that GetTickCount() API is not high-resolution).
    ///
    /// Linux implementation is based on \ref gettimeofday() function.
    ///
    /// Usage:
    ///
    /// ~~~{.c}
    /// platform::performance_counter timer;
    /// {
    ///     platform::performance_scope scope(timer);
    ///     // some computing here
    /// }
    /// printf("computing completed, elapsed %g seconds", timer.get_seconds());
    /// ~~~
    class performance_counter
    {
    public:
        typedef performance_counter class_type;

#if defined PLATFORM_WIN32
        typedef __int64           period_count_type;
        typedef __int64           interval_type;
#elif defined PLATFORM_LINUX
        typedef struct timeval    period_count_type;
        typedef ssize_t           interval_type;
#else
        typedef ssize_t           period_count_type
        typedef ssize_t           interval_type;
#endif
        typedef double            sec_interval_type;
        typedef period_count_type value_type;

        /// Start measure
        void start();
        /// Stop measure
        void stop();
        /// Restart measure
        void restart();

        /// Elapsed platform-dependent ticks count in measured interval.
        period_count_type get_period_count() const;
        /// Elapsed seconds count in measured interval
        sec_interval_type get_seconds() const;
        /// Elapsed milliseconds count in measured interval.
        interval_type     get_milliseconds() const;
        /// Elapsed microseconds count in measured interval.
        interval_type     get_microseconds() const;

    private:
        value_type start_value;
        value_type end_value;
    };

#if defined PLATFORM_WIN32

    namespace detail
    {
        inline performance_counter::interval_type query_frequency()
        {
            performance_counter::interval_type frequency;
            if( !::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency)) || frequency == 0)
            {
                // Prevent division by zero if QueryPerformanceCounter() API doesn't supported by OS
                frequency = 1000;
            }
            return frequency;
        }

        inline performance_counter::interval_type frequency()
        {
			static performance_counter::interval_type frequency_ = query_frequency();
            assert(0 != frequency_);
            return frequency_;
        }

        inline void qpc(performance_counter::value_type &epoch)
        {
            ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&epoch));
        }

        inline void gtc(performance_counter::value_type &epoch)
        {
            epoch = static_cast<performance_counter::value_type>(::GetTickCount());
        }

        typedef void (*measure_fn_type)(performance_counter::value_type &);
        inline measure_fn_type get_measure_fn()
        {
            measure_fn_type fn;
			performance_counter::value_type frequency;

            if(QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency)))
                fn = qpc;
            else
                fn = gtc;

            return fn;
        }
    }

	inline void measure(performance_counter::value_type &epoch)
    {
		static detail::measure_fn_type fn = detail::get_measure_fn();
        fn(epoch);
    }

	inline performance_counter::sec_interval_type performance_counter::get_seconds() const
    {
		return static_cast<sec_interval_type>(get_period_count()) / detail::frequency();
    }

	inline performance_counter::interval_type     performance_counter::get_milliseconds() const
    {
        interval_type result;
        interval_type count = get_period_count();

        if(count < 0x20C49BA5E353F7LL)
            result = (count*interval_type(1000))/detail::frequency();
        else
            result = (count/detail::frequency())*interval_type(1000);

        return result;
    }

    inline performance_counter::interval_type     performance_counter::get_microseconds() const
    {
        interval_type   result;
        interval_type   count   =   get_period_count();

        if(count < 0x8637BD05AF6LL)
            result = (count*interval_type(1000000))/detail::frequency();
        else
            result = (count/detail::frequency())*interval_type(1000000);

        return result;
    }

#elif defined PLATFORM_LINUX

    namespace detail
    {
        typedef performance_counter::sec_interval_type sec_interval_type;
        inline sec_interval_type get_seconds(const timeval &start, const timeval &end)
        {
            //"end before start: stop() must be called after start()"
            assert(start.tv_sec <= end.tv_sec);

            long    secs    =   end.tv_sec - start.tv_sec;
            long    usecs   =   end.tv_usec - start.tv_usec;

            assert(usecs >= 0 || secs > 0);

            return sec_interval_type(secs) + sec_interval_type(usecs)/(1000*1000);
        }

        inline performance_counter::interval_type get_milliseconds(const timeval &start, const timeval &end)
        {
            //"end before start: stop() must be called after start()"
            assert(start.tv_sec <= end.tv_sec);

            long    secs    =   end.tv_sec - start.tv_sec;
            long    usecs   =   end.tv_usec - start.tv_usec;

            assert(usecs >= 0 || secs > 0);

            return secs*1000 + usecs/1000;
        }

        inline performance_counter::interval_type get_microseconds(const timeval &start, const timeval &end)
        {
            //"end before start: stop() must be called after start()"
            assert(start.tv_sec <= end.tv_sec);

            long    secs    =   end.tv_sec - start.tv_sec;
            long    usecs   =   end.tv_usec - start.tv_usec;

            assert(usecs >= 0 || secs > 0);

            return secs*(1000*1000) + usecs;
        }
    }

    inline void operator+=(timeval &acc, const timeval &addition)
    {
        acc.tv_sec += addition.tv_sec;
        suseconds_t usecs = acc.tv_usec + addition.tv_usec;

        const int microsecondsInSecond = 1000*1000;
        acc.tv_sec += usecs / microsecondsInSecond;
        acc.tv_usec = usecs % microsecondsInSecond;
    }

    inline void operator-=(timeval &acc, const timeval &subtraction)
    {
        acc.tv_sec -= subtraction.tv_sec;
        if (acc.tv_usec >= subtraction.tv_usec)
        {
            acc.tv_usec -= subtraction.tv_usec;
        }
        else
        {
            suseconds_t usecs = subtraction.tv_usec - acc.tv_usec;
            const int microsecondsInSecond = 1000*1000;
            acc.tv_sec -= 1 + usecs / microsecondsInSecond;
            acc.tv_usec = microsecondsInSecond - usecs%microsecondsInSecond;
        }
    }

    inline void measure(performance_counter::value_type &epoch)
    {
        gettimeofday(&epoch, NULL);
    }

    inline performance_counter::sec_interval_type performance_counter::get_seconds() const
    {
        return detail::get_seconds(start_value, end_value);
    }

    inline performance_counter::interval_type performance_counter::get_milliseconds() const
    {
        return detail::get_milliseconds(start_value, end_value);
    }

    inline performance_counter::interval_type performance_counter::get_microseconds() const
    {
        return detail::get_microseconds(start_value, end_value);
    }

#endif

    inline void performance_counter::start()
    {
        measure(start_value);
        end_value = start_value;
    }

    inline void performance_counter::stop()
    {
        measure(end_value);
    }

    inline void performance_counter::restart()
    {
        measure(start_value);
        end_value = start_value;
    }

    inline performance_counter::period_count_type performance_counter::get_period_count() const
    {
        period_count_type result = end_value;
        result -= start_value;
        return result;
    }



    /// \brief Timer with pause support (value accumulation)
    ///
    /// Subsequent calls to start()/stop() functions will lead
    /// to accumulate time in accumulation member.
    /// You can call reset() function for drop accumulated value to zero.
    ///
    /// ~~~{.c}
    /// platform::acc_performance_counter timer;
    /// const int repeatCount = 1000;
    /// for (int i = 0; i < repeatCount; ++i)
    /// {
    ///     platform::acc_performance_scope scope(timer);
    ///     // some computing here
    /// }
    /// printf("computing completed, elapsed %g seconds (%g per call)", timer.get_seconds(), timer.get_seconds()/repeatCount);
    /// ~~~
    template <class timer_type>
    class accumulation_performance_counter
    {
    public:
        typedef accumulation_performance_counter class_type;
        typedef typename timer_type::value_type value_type;
        typedef typename timer_type::interval_type interval_type;
        typedef typename timer_type::sec_interval_type sec_interval_type;
        typedef typename timer_type::period_count_type period_count_type;

        accumulation_performance_counter() : accumulation() {}

        /// Continue measure
        void start();
        /// Pause measure
        void stop();
        /// Reset measure
        void reset();
        /// Decrease measure by specified overhead
        void decrease(period_count_type overhead);

        /// Elapsed platform-dependent ticks count in measured interval.
        period_count_type get_period_count() const { return accumulation; }
        /// Elapsed seconds count in measured interval
        sec_interval_type get_seconds() const;
        /// Elapsed milliseconds count in measured interval.
        interval_type     get_milliseconds() const;
        /// Elapsed microseconds count in measured interval.
        interval_type     get_microseconds() const;

    private:
        timer_type timer;
        period_count_type accumulation;
    };

    // Operations
    template <class timer_type>
    inline void accumulation_performance_counter<timer_type>::start()
    {
         timer.start();
    }

    template <class timer_type>
    inline void accumulation_performance_counter<timer_type>::stop()
    {
        timer.stop();
        accumulation += timer.get_period_count();
    }

    template <class timer_type>
    inline void accumulation_performance_counter<timer_type>::reset()
    {
        accumulation = period_count_type();
    }

    template <class timer_type>
    inline void accumulation_performance_counter<timer_type>::decrease(
        typename accumulation_performance_counter<timer_type>::period_count_type overhead)
    {
        accumulation -= overhead;
    }

#if defined PLATFORM_WIN32

    template <class timer_type>
    inline typename accumulation_performance_counter<timer_type>::sec_interval_type
    accumulation_performance_counter<timer_type>::get_seconds() const
    {
        return (sec_interval_type)get_period_count()/detail::frequency();
    }

    template <class timer_type>
    inline typename accumulation_performance_counter<timer_type>::interval_type
    accumulation_performance_counter<timer_type>::get_milliseconds() const
    {
        interval_type result;
        interval_type count = get_period_count();

        if(count < 0x20C49BA5E353F7LL)
            result = (count*interval_type(1000))/detail::frequency();
        else
            result = (count/detail::frequency())*interval_type(1000);

        return result;
    }

    template <class timer_type>
    inline typename accumulation_performance_counter<timer_type>::interval_type
    accumulation_performance_counter<timer_type>::get_microseconds() const
    {
        interval_type   result;
        interval_type   count   =   get_period_count();

        if(count < 0x8637BD05AF6LL)
            result = (count*interval_type(1000000))/detail::frequency();
        else
            result = (count/detail::frequency())*interval_type(1000000);

        return result;
    }

#elif defined PLATFORM_LINUX

    template <class timer_type>
    inline typename accumulation_performance_counter<timer_type>::sec_interval_type
    accumulation_performance_counter<timer_type>::get_seconds() const
    {
        period_count_type accumulated = get_period_count();
        return sec_interval_type(accumulated.tv_sec) + sec_interval_type(accumulated.tv_usec)/(1000*1000);
    }

    template <class timer_type>
    inline typename accumulation_performance_counter<timer_type>::interval_type
    accumulation_performance_counter<timer_type>::get_milliseconds() const
    {
        period_count_type accumulated = get_period_count();
        return accumulated.tv_sec*1000 + accumulated.tv_usec/1000;
    }

    template <class timer_type>
    inline typename accumulation_performance_counter<timer_type>::interval_type
    accumulation_performance_counter<timer_type>::get_microseconds() const
    {
        period_count_type accumulated = get_period_count();
        return accumulated.tv_sec*1000*1000 + accumulated.tv_usec;
    }

#endif
}

namespace platform
{
    /// \brief Scope for default \ref performance_counter timer
    typedef timer_scope<performance_counter> performance_scope;
    /// \brief Initialized timer based on \ref performance_counter timer
    typedef timer_initialiser<performance_counter> performance_initialized_timer;

    /// \brief Timer with accumulation based on default \ref performance_counter timer
    typedef accumulation_performance_counter<performance_counter> acc_performance_counter;
    /// \brief Scope for timer with accumulation
    typedef timer_scope<acc_performance_counter> acc_performance_scope;
    /// \brief Initialized timer with accumulation based on \ref acc_performance_counter timer
    typedef timer_initialiser<acc_performance_counter> acc_performance_initialized_timer;
}

#endif
