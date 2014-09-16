///
/// \file
///
/// \brief  Реализация таймера, использующего \c QueryPerformanceCounter().
///
/// Реализация взята из статьи Mattew Wilson
/// [Win32 Performance Measurement Options](http://www.drdobbs.com/windows/win32-performance-measurement-options/184416651)
/// и немного переработана.
///

#ifndef __PERFOMANCE_COUNTER_H__
#define __PERFOMANCE_COUNTER_H__

#ifndef assert
#include <cassert>
#endif

namespace platform
{
    /// \brief Инициализатор таймеров.
    ///
    /// В целях производительности обычная реализация таймеров
    /// не инициализирует внутренние данные, ожидая вызова
    /// start()/stop() для их инициализации. Если все же
    /// необходимо иметь инициализированный таймер,
    /// можно воспользоваться этим шаблоном.
    template <typename Counter>
    class timer_initialiser : public Counter
    {
    public:
        typedef Counter counter_type;
        typedef timer_initialiser<Counter> class_type;

    public:
        /// Инициализация таймера вызовом start()/stop()
        timer_initialiser()
        {
            counter_type::start();
            counter_type::stop();
        }
    };


    /// \brief Таймер на область видимости.
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

        // Метод константный чтобы гарантировать
        // доступность только операции stop().
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

#ifdef _WIN32

namespace platform
{
    /// \brief Реализация таймера на основе PerformanceCounter.
    ///
    /// По умолчанию для измерения времени используется
    /// \c QueryPerformanceCounter(). Если в системе нет
    /// счетчиков производительности, для измерения
    /// используется функция \c GetTickCount(), которая
    /// менее точна...
    ///
    /// При измерениях функция \c QueryPerformanceFrequency()
    /// вызывается лишь однажды, далее ее значение кэшируется
    /// и используется в дальнейших вычислениях.
    class performance_counter
    {
    public:
        typedef performance_counter class_type;
        typedef __int64 value_type;
        typedef __int64 interval_type;
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

        /// \brief Прошедшее количество "тиков" в измеренном периоде.
        interval_type get_period_count() const;

        /// \brief Количество секунд в измеренном периоде.
        sec_interval_type get_seconds() const;

        /// \brief Количество миллисекунд в измеренном периоде.
        interval_type get_milliseconds() const;

        /// \brief Количество микросекунд в измеренном периоде.
        interval_type get_microseconds() const;

        static interval_type frequency();
    private:
        typedef void (*measure_fn_type)(value_type&);

        static interval_type query_frequency();
        static void qpc(value_type &epoch); ///< Получение времени посредством QueryPerformanceCounter()
        static void gtc(value_type &epoch); ///< Получение времени посредством GetTickCount()
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
            // Предотвращаем деление на нуль, если
            // performance counter отсутствует в системе.
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
}

#elif defined __GNUC__

#include <sys/time.h>

namespace platform
{
    class performance_counter
    {
    public:
        typedef struct timeval      epoch_type;
        typedef ssize_t             interval_type;
        typedef performance_counter class_type;

    public:

        void    start();
        void    stop();
        void    restart();

    public:
        static epoch_type       get_epoch();

        static interval_type    get_seconds(epoch_type start, epoch_type end);
        static interval_type    get_milliseconds(epoch_type start, epoch_type end);
        static interval_type    get_microseconds(epoch_type start, epoch_type end);

        interval_type   get_period_count() const;
        interval_type   get_seconds() const;
        interval_type   get_milliseconds() const;
        interval_type   get_microseconds() const;

        interval_type   stop_get_period_count_and_restart();

        interval_type   stop_get_seconds_and_restart();

        interval_type   stop_get_milliseconds_and_restart();

        interval_type   stop_get_microseconds_and_restart();

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

    inline /* static */ performance_counter::interval_type performance_counter::get_seconds(performance_counter::epoch_type start, performance_counter::epoch_type end)
    {
        assert(start.tv_sec <= end.tv_sec); //"end before start: stop() must be called after start()"

        long    secs    =   end.tv_sec - start.tv_sec;
        long    usecs   =   end.tv_usec - start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        return secs + usecs / (1000 * 1000);
    }

    inline /* static */ performance_counter::interval_type performance_counter::get_milliseconds(performance_counter::epoch_type start, performance_counter::epoch_type end)
    {
        assert(start.tv_sec <= end.tv_sec); //"end before start: stop() must be called after start()"

        long    secs    =   end.tv_sec - start.tv_sec;
        long    usecs   =   end.tv_usec - start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        return secs * 1000 + usecs / 1000;
    }

    inline /* static */ performance_counter::interval_type performance_counter::get_microseconds(performance_counter::epoch_type start, performance_counter::epoch_type end)
    {
        assert(start.tv_sec <= end.tv_sec); //"end before start: stop() must be called after start()"

        long    secs    =   end.tv_sec - start.tv_sec;
        long    usecs   =   end.tv_usec - start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        return secs * (1000 * 1000) + usecs;
    }

    inline performance_counter::interval_type performance_counter::get_period_count() const
    {
        return get_microseconds();
    }

    inline performance_counter::interval_type performance_counter::get_seconds() const
    {
        assert(m_start.tv_sec <= m_end.tv_sec); //"end before start: stop() must be called after start()"

        long    secs    =   m_end.tv_sec - m_start.tv_sec;
        long    usecs   =   m_end.tv_usec - m_start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        return secs + usecs / (1000 * 1000);
    }

    inline performance_counter::interval_type performance_counter::get_milliseconds() const
    {
        assert(m_start.tv_sec <= m_end.tv_sec); //"end before start: stop() must be called after start()"

        long    secs    =   m_end.tv_sec - m_start.tv_sec;
        long    usecs   =   m_end.tv_usec - m_start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        return secs * 1000 + usecs / 1000;
    }

    inline performance_counter::interval_type performance_counter::get_microseconds() const
    {
        assert(m_start.tv_sec <= m_end.tv_sec); //"end before start: stop() must be called after start()"

        long    secs    =   m_end.tv_sec - m_start.tv_sec;
        long    usecs   =   m_end.tv_usec - m_start.tv_usec;

        assert(usecs >= 0 || secs > 0);

        return secs * (1000 * 1000) + usecs;
    }
}

#endif

namespace platform
{
    typedef timer_scope<performance_counter> performance_scope;
    typedef timer_initialiser<performance_counter> performance_initialized_timer;

    ///
    /// \brief Реализация таймеров с паузой (накоплением значения)
    ///        на основе других таймеров с одним интерфейсом.
    ///
    /// Последовательные вызовы функций start() и stop()
    /// приводят к накоплению значений времени в поле
    /// accumulation. Для сброса этого значения в нуль
    /// необходимо вызвать функцию reset().
    template <class timer_type>
    class accumulation_performance_counter
    {
    public:
        typedef accumulation_performance_counter class_type;
        typedef typename timer_type::value_type value_type;
        typedef typename timer_type::interval_type interval_type;
        typedef typename timer_type::sec_interval_type sec_interval_type;

        accumulation_performance_counter() : accumulation(0) {}

        static void class_init() { timer_type::class_init(); }
        static void class_uninit() { timer_type::class_uninit(); }

        void start();
        void stop();
        void reset();
        void decrease(interval_type overhead);

        /// \brief Прошедшее количество "тиков" в измеренном периоде.
        interval_type get_period_count() const { return accumulation; }

        /// \brief Количество секунд в измеренном периоде.
        sec_interval_type get_seconds() const;

        /// \brief Количество миллисекунд в измеренном периоде.
        interval_type get_milliseconds() const;

        /// \brief Количество микросекунд в измеренном периоде.
        interval_type get_microseconds() const;

        static interval_type frequency() { return timer_type::frequency(); }

    private:
        timer_type timer;
        interval_type accumulation;
    };

    typedef accumulation_performance_counter<performance_counter> acc_performance_counter;
    typedef timer_scope<accumulation_performance_counter<performance_counter>> acc_performance_scope;
    typedef timer_initialiser<accumulation_performance_counter<performance_counter>> acc_performance_initialized_timer;


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

#endif

