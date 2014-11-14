#pragma once

#include <atomic>
#include <condition_variable>
#include <stdexcept>

namespace utility
{
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

        bool waitForEvent(int milliseconds = 1000)
        {
            lock_type guard(_mutex);
            if (milliseconds > 0)
            {
                return std::cv_status::timeout != _condition.wait_for(guard, std::chrono::milliseconds(milliseconds));
            }
            else
            {
                _condition.wait(guard);
                return true;
            }
        }

        bool checkFired(int milliseconds = 1000)
        {
            return waitForEvent(milliseconds) && isEventFired();
        }

        void eventSubscription(const Args &arg)
        {
            lock_type guard(_mutex);
            _eventFired = true;
            ++_countFired;
            _arguments.push_back(arg);
            _condition.notify_one();
        }

        const Args &getArg(int index) const
        {
            return _arguments[index];
        }
    private:
        mutex_type _mutex;
        std::condition_variable _condition;
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

        bool waitForEvent(int milliseconds = 1000)
        {
            lock_type guard(_mutex);
            if (milliseconds > 0)
            {
                return std::cv_status::timeout != _condition.wait_for(guard, std::chrono::milliseconds(milliseconds));
            }
            else
            {
                _condition.wait(guard);
                return true;
            }
        }

        bool checkFired(int milliseconds = 1000)
        {
            return waitForEvent(milliseconds) && isEventFired();
        }

        void eventSubscription()
        {
            lock_type guard(_mutex);
            _eventFired = true;
            ++_countFired;
            _condition.notify_one();
        }

    private:
        mutex_type _mutex;
        std::condition_variable _condition;
        bool _eventFired;
        std::atomic<int> _countFired;
    };
}