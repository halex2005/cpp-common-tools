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

namespace platform
{
    class performance_counter
    {
    public:
        typedef performance_counter class_type;
        typedef __int64 value_type;
        typedef __int64 interval_type;
        typedef __int64 period_count_type;
        typedef float sec_interval_type;

        static void class_init()
        {
            class_type instance;
            instance.start();
        }
        static void class_uninit()
        {}

        void start();
        void stop();

        period_count_type get_period_count() const;
        sec_interval_type get_seconds() const;
        interval_type     get_milliseconds() const;
        interval_type     get_microseconds() const;

        static interval_type frequency();
    private:
        typedef void (*measure_fn_type)(value_type&);

        static interval_type query_frequency();
        static void qpc(value_type &epoch); ///< Get ticks using QueryPerformanceCounter() API
        static void gtc(value_type &epoch); ///< Get ticks using GetTickCount() API
        static measure_fn_type get_measure_fn();
        static void measure(value_type &epoch);

        // Данные
        value_type  start_value;
        value_type  end_value;
    };

    inline performance_counter::interval_type performance_counter::query_frequency()
    {
        interval_type frequency;
        if( !::QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency)) || frequency == 0)
        {
            // Prevent division by zero if QueryPerformanceCounter() API doesn't supported by OS
            frequency = 1000;
        }
        return frequency;
    }

    inline performance_counter::interval_type performance_counter::frequency()
    {
        static interval_type frequency_ = query_frequency();
        assert(0 != frequency_);
        return frequency_;
    }

    inline void performance_counter::qpc(value_type &epoch)
    {
        ::QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&epoch));
    }

    inline void performance_counter::gtc(value_type &epoch)
    {
        epoch = static_cast<value_type>(::GetTickCount());
    }

    inline performance_counter::measure_fn_type performance_counter::get_measure_fn()
    {
        measure_fn_type fn;
        value_type frequency;

        if(QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&frequency)))
            fn = qpc;
        else
            fn = gtc;

        return fn;
    }

    inline void performance_counter::measure(value_type &epoch)
    {
        static measure_fn_type fn = get_measure_fn();
        fn(epoch);
    }

    // Operations
    inline void performance_counter::start()
    {
        measure(start_value);
    }

    inline void performance_counter::stop()
    {
        measure(end_value);
    }

    // Attributes
    inline performance_counter::interval_type performance_counter::get_period_count() const
    {
        return static_cast<interval_type>(end_value - start_value);
    }

    inline performance_counter::sec_interval_type performance_counter::get_seconds() const
    {
        return (sec_interval_type)get_period_count()/frequency();
    }

    inline performance_counter::interval_type performance_counter::get_milliseconds() const
    {
        interval_type result;
        interval_type count = get_period_count();

        if(count < 0x20C49BA5E353F7LL)
            result = (count*interval_type(1000))/frequency();
        else
            result = (count/frequency())*interval_type(1000);

        return result;
    }

    inline performance_counter::interval_type performance_counter::get_microseconds() const
    {
        interval_type   result;
        interval_type   count   =   get_period_count();

        if(count < 0x8637BD05AF6LL)
            result = (count*interval_type(1000000))/frequency();
        else
            result = (count/frequency())*interval_type(1000000);

        return result;
    }

    template <class timer_type>
    class accumulation_performance_counter
    {
    public:
        typedef accumulation_performance_counter class_type;
        typedef typename timer_type::value_type value_type;
        typedef typename timer_type::interval_type interval_type;
        typedef typename timer_type::sec_interval_type sec_interval_type;
        typedef typename timer_type::period_count_type period_count_type;

        accumulation_performance_counter() : accumulation(0) {}

        static void class_init() { timer_type::class_init(); }
        static void class_uninit() { timer_type::class_uninit(); }

        void start();
        void stop();
        void reset();
        void decrease(interval_type overhead);

        /// Elapsed platform-dependent ticks count in measured interval.
        period_count_type get_period_count() const { return accumulation; }
        sec_interval_type get_seconds() const;
        interval_type     get_milliseconds() const;
        interval_type     get_microseconds() const;

        static interval_type frequency() { return timer_type::frequency(); }

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
        accumulation = interval_type(0);
    }

    template <class timer_type>
    inline void accumulation_performance_counter<timer_type>::decrease(typename accumulation_performance_counter<timer_type>::interval_type overhead)
    {
        accumulation -= overhead;
    }

    // Attributes
    template <class timer_type>
    inline typename accumulation_performance_counter<timer_type>::sec_interval_type accumulation_performance_counter<timer_type>::get_seconds() const
    {
        return (sec_interval_type)get_period_count()/timer.frequency();
    }

    template <class timer_type>
    inline typename accumulation_performance_counter<timer_type>::interval_type accumulation_performance_counter<timer_type>::get_milliseconds() const
    {
        interval_type result;
        interval_type count = get_period_count();

        if(count < 0x20C49BA5E353F7LL)
            result = (count*interval_type(1000))/timer.frequency();
        else
            result = (count/timer.frequency())*interval_type(1000);

        return result;
    }

    template <class timer_type>
    inline typename accumulation_performance_counter<timer_type>::interval_type accumulation_performance_counter<timer_type>::get_microseconds() const
    {
        interval_type   result;
        interval_type   count   =   get_period_count();

        if(count < 0x8637BD05AF6LL)
            result = (count*interval_type(1000000))/timer.frequency();
        else
            result = (count/timer.frequency())*interval_type(1000000);

        return result;
    }
}

#elif defined PLATFORM_LINUX

#include <sys/time.h>

namespace platform
{
    class performance_counter
    {
    public:
        typedef struct timeval      epoch_type;
        typedef epoch_type          period_count_type;
        typedef ssize_t             value_type;
        typedef ssize_t             interval_type;
        typedef float               sec_interval_type;
        typedef performance_counter class_type;

    public:
        void    start();
        void    stop();
        void    restart();

    public:
        static epoch_type         get_epoch();
        static sec_interval_type  get_seconds(epoch_type start, epoch_type end);
        static interval_type      get_milliseconds(epoch_type start, epoch_type end);
        static interval_type      get_microseconds(epoch_type start, epoch_type end);

        period_count_type get_period_count() const;
        sec_interval_type get_seconds() const;
        interval_type     get_milliseconds() const;
        interval_type     get_microseconds() const;

        period_count_type stop_get_period_count_and_restart();
        sec_interval_type stop_get_seconds_and_restart();
        interval_type     stop_get_milliseconds_and_restart();
        interval_type     stop_get_microseconds_and_restart();

    private:
        static void measure_(epoch_type &epoch);

    private:
        epoch_type  m_start;
        epoch_type  m_end;
    };

    inline /* static */ void performance_counter::measure_(performance_counter::epoch_type &epoch)
    {
        gettimeofday(&epoch, NULL);
    }

    inline void performance_counter::start()
    {
        measure_(m_start);

        m_end = m_start;
    }

    inline void performance_counter::stop()
    {
        measure_(m_end);
    }

    inline void performance_counter::restart()
    {
        measure_(m_start);
        m_end = m_start;
    }

    inline /* static */ performance_counter::epoch_type performance_counter::get_epoch()
    {
        epoch_type epoch;

        measure_(epoch);

        return epoch;
    }

    inline /* static */ performance_counter::sec_interval_type performance_counter::get_seconds(performance_counter::epoch_type start, performance_counter::epoch_type end)
    {
        //"end before start: stop() must be called after start()"
        assert(start.tv_sec <= end.tv_sec);

        long    secs    =   end.tv_sec - start.tv_sec;
        long    usecs   =   end.tv_usec - start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        return sec_interval_type(secs) + sec_interval_type(usecs)/(1000*1000);
    }

    inline /* static */ performance_counter::interval_type performance_counter::get_milliseconds(performance_counter::epoch_type start, performance_counter::epoch_type end)
    {
        //"end before start: stop() must be called after start()"
        assert(start.tv_sec <= end.tv_sec);

        long    secs    =   end.tv_sec - start.tv_sec;
        long    usecs   =   end.tv_usec - start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        return secs * 1000 + usecs / 1000;
    }

    inline /* static */ performance_counter::interval_type performance_counter::get_microseconds(performance_counter::epoch_type start, performance_counter::epoch_type end)
    {
        //"end before start: stop() must be called after start()"
        assert(start.tv_sec <= end.tv_sec);

        long    secs    =   end.tv_sec - start.tv_sec;
        long    usecs   =   end.tv_usec - start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        return secs*(1000*1000) + usecs;
    }

    inline performance_counter::period_count_type performance_counter::get_period_count() const
    {
        //"end before start: stop() must be called after start()"
        assert(m_start.tv_sec <= m_end.tv_sec);

        long    secs    =   m_end.tv_sec - m_start.tv_sec;
        long    usecs   =   m_end.tv_usec - m_start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        period_count_type result;
        result.tv_sec = secs;
        result.tv_usec = usecs;
        return result;
    }

    inline performance_counter::sec_interval_type performance_counter::get_seconds() const
    {
        //"end before start: stop() must be called after start()"
        assert(m_start.tv_sec <= m_end.tv_sec);

        long    secs    =   m_end.tv_sec - m_start.tv_sec;
        long    usecs   =   m_end.tv_usec - m_start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        return sec_interval_type(secs) + sec_interval_type(usecs)/(1000*1000);
    }

    inline performance_counter::interval_type performance_counter::get_milliseconds() const
    {
        //"end before start: stop() must be called after start()"
        assert(m_start.tv_sec <= m_end.tv_sec);

        long    secs    =   m_end.tv_sec - m_start.tv_sec;
        long    usecs   =   m_end.tv_usec - m_start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        return secs*1000 + usecs/1000;
    }

    inline performance_counter::interval_type performance_counter::get_microseconds() const
    {
        //"end before start: stop() must be called after start()"
        assert(m_start.tv_sec <= m_end.tv_sec);

        long    secs    =   m_end.tv_sec - m_start.tv_sec;
        long    usecs   =   m_end.tv_usec - m_start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        return secs * (1000 * 1000) + usecs;
    }

    template <class timer_type>
    class accumulation_performance_counter
    {
    public:
        typedef accumulation_performance_counter class_type;
        typedef typename timer_type::epoch_type epoch_type;
        typedef typename timer_type::value_type value_type;
        typedef typename timer_type::interval_type interval_type;
        typedef typename timer_type::sec_interval_type sec_interval_type;
        typedef typename timer_type::period_count_type period_count_type;

        accumulation_performance_counter() : accumulation() {}

        static void class_init() { timer_type::class_init(); }
        static void class_uninit() { timer_type::class_uninit(); }

        void start();
        void stop();
        void reset();
        void decrease(period_count_type overhead);

        period_count_type get_period_count() const { return accumulation; }
        sec_interval_type get_seconds() const;
        interval_type     get_milliseconds() const;
        interval_type     get_microseconds() const;

        static interval_type frequency() { return timer_type::frequency(); }

    private:
        timer_type timer;
        period_count_type accumulation;
    };

    inline void operator+=(timeval &acc, const timeval &addition)
    {
        acc.tv_sec += addition.tv_sec;
        suseconds_t usecs = acc.tv_usec + addition.tv_usec;

        int microsecondsInSecond = 1000*1000;
        acc.tv_sec += usecs / microsecondsInSecond;
        acc.tv_usec = usecs % microsecondsInSecond;
    }

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
        accumulation = interval_type(0);
    }

    template <class timer_type>
    inline void accumulation_performance_counter<timer_type>::decrease(typename accumulation_performance_counter<timer_type>::period_count_type overhead)
    {
        accumulation -= overhead;
    }

    // Attributes
    template <class timer_type>
    inline typename accumulation_performance_counter<timer_type>::sec_interval_type accumulation_performance_counter<timer_type>::get_seconds() const
    {
        epoch_type accumulated = get_period_count();
        return sec_interval_type(accumulated.tv_sec) + sec_interval_type(accumulated.tv_usec)/(1000*1000);
    }

    template <class timer_type>
    inline typename accumulation_performance_counter<timer_type>::interval_type accumulation_performance_counter<timer_type>::get_milliseconds() const
    {
        epoch_type accumulated = get_period_count();
        return accumulated.tv_sec*1000 + accumulated.tv_usec/1000;
    }

    template <class timer_type>
    inline typename accumulation_performance_counter<timer_type>::interval_type accumulation_performance_counter<timer_type>::get_microseconds() const
    {
        epoch_type accumulated = get_period_count();
        return accumulated.tv_sec*1000*1000 + accumulated.tv_usec;
    }
}

#endif

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

#ifdef __DOXYGEN_RUNNING__

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
    /// ~~~{.c++}
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
        typedef platfrom-dependent period_count_type;
        typedef ssize_t interval_type;
        typedef float sec_interval_type;

        /// \brief Start measure
        void start();
        /// \brief Stop measure
        void stop();

        /// \brief Elapsed platform-dependent ticks count in measured interval.
        period_count_type get_period_count() const;

        /// \brief Elapsed seconds count in measured interval
        sec_interval_type get_seconds() const;

        /// \brief Elapsed milliseconds count in measured interval.
        interval_type     get_milliseconds() const;

        /// \brief Elapsed microseconds count in measured interval.
        interval_type     get_microseconds() const;
    };

    /// \brief Timer with pause support (value accumulation)
    ///
    /// Subsequent calls to start()/stop() functions will lead
    /// to accumulate time in accumilation member.
    /// You can call reset() function for drop accumulated value to zero.
    template <class timer_type>
    class accumulation_performance_counter
    {
    public:
        typedef typename timer_type::interval_type interval_type;
        typedef typename timer_type::sec_interval_type sec_interval_type;
        typedef typename timer_type::period_count_type period_count_type;

        /// Continue measure
        void start();

        /// Pause measure
        void stop();

        /// Reset measure
        void reset();

        /// Decrease specified overhead
        void decrease(period_count_type overhead);

        /// Elapsed platform-dependent ticks count in measured interval.
        period_count_type get_period_count() const;

        /// \brief Elapsed seconds count in measured interval
        sec_interval_type get_seconds() const;

        /// \brief Elapsed milliseconds count in measured interval.
        interval_type     get_milliseconds() const;

        /// \brief Elapsed microseconds count in measured interval.
        interval_type     get_microseconds() const;
    };
}

#endif

#endif
