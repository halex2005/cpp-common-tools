#pragma once

#include <atomic>
#include <condition_variable>
#include <thread>

namespace utility
{
    class AutoResetEvent
    {
    public:
        typedef std::mutex mutex_type;
        typedef std::unique_lock<mutex_type> lock_type;

        explicit AutoResetEvent(bool initial = false)
            : _flag(initial)
        {
        }

        void Set()
        {
            lock_type guard(_protect);
            _flag = true;
            _signal.notify_one();
        }

        void Reset()
        {
            lock_type guard(_protect);
            _flag = false;
        }

        bool WaitOne()
        {
            lock_type guard(_protect);
            // prevent spurious wakeups from doing harm
            while (!_flag)
                _signal.wait(guard);

            // waiting resets the flag
            _flag = false;
            return true;
        }

        bool WaitOne(std::chrono::milliseconds &timeout)
        {
            using namespace std::chrono;
            lock_type guard(_protect);
            steady_clock::time_point borderPoint = steady_clock::now() + timeout;
            while (!_flag)
            {
                if (borderPoint < steady_clock::now())
                    return false;
                _signal.wait_for(guard, timeout);
            }

            // waiting resets the flag
            _flag = false;
            return true;
        }

    private:
        // non-copyable
        AutoResetEvent(const AutoResetEvent&) = delete;
        AutoResetEvent& operator=(const AutoResetEvent&) = delete;

        bool _flag;
        std::mutex _protect;
        std::condition_variable _signal;
    };

    template <class Predicate>
    bool wait_for_condition(Predicate predicate, std::chrono::milliseconds timeout = std::chrono::seconds(1))
    {
        using namespace std::chrono;
        auto borderPoint = steady_clock::now() + timeout;
        while (steady_clock::now() < borderPoint)
        {
            if (predicate())
                return true;
            std::this_thread::sleep_for(milliseconds(1));
        }
        return false;
    }

    template <typename Args>
    class event_test
    {
    public:
        typedef std::mutex mutex_type;
        typedef std::unique_lock<mutex_type> lock_type;

        event_test()
            : _eventFired(false)
            , _countFired()
        {
        }

        bool isEventFired() const { return _eventFired; }
        int countFired() const { return _countFired; }

        bool waitForEvent(std::chrono::milliseconds milliseconds = std::chrono::milliseconds(1000))
        {
            return milliseconds.count() > 0
                ? _event.WaitOne(milliseconds)
                : _event.WaitOne();
        }

        bool checkFired(std::chrono::milliseconds milliseconds = std::chrono::milliseconds(1000))
        {
            return waitForEvent(milliseconds) && isEventFired();
        }

        void eventSubscription(const Args &arg)
        {
            lock_type guard(_mutex);
            _eventFired = true;
            ++_countFired;
            _arguments.push_back(arg);
            _event.Set();
        }

        const Args &getArg(int index) const
        {
            return _arguments[index];
        }
    private:
        mutex_type _mutex;
        AutoResetEvent _event;
        bool _eventFired;
        std::atomic<int> _countFired;
        std::vector<Args> _arguments;
    };

    template<>
    class event_test<void>
    {
    public:
        typedef std::mutex mutex_type;
        typedef std::unique_lock<mutex_type> lock_type;

        event_test()
            : _eventFired(false)
            , _countFired()
        {
        }

        bool isEventFired() const { return _eventFired; }
        int countFired() const { return _countFired; }

        bool waitForEvent(std::chrono::milliseconds milliseconds = std::chrono::milliseconds(1000))
        {
            return milliseconds.count() > 0
                ? _event.WaitOne(milliseconds)
                : _event.WaitOne();
        }

        bool checkFired(std::chrono::milliseconds milliseconds = std::chrono::milliseconds(1000))
        {
            return waitForEvent(milliseconds) && isEventFired();
        }

        void eventSubscription()
        {
            lock_type guard(_mutex);
            _eventFired = true;
            ++_countFired;
            _event.Set();
        }

    private:
        mutex_type _mutex;
        AutoResetEvent _event;
        bool _eventFired;
        std::atomic<int> _countFired;
    };
}