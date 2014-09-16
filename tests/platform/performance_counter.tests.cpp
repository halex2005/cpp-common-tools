#include <catch/catch.hpp>
#include <platform/performance_counter.h>
#include <thread>

struct timer_mock
{
    timer_mock() : start_count(), stop_count() {}

    void start() { ++start_count; }
    void stop() { ++stop_count; }

    int start_count;
    int stop_count;
};


TEST_CASE("performance counter", "[perfcounter][platform]")
{
    SECTION("timer initializer")
    {
        platform::timer_initialiser<timer_mock> initialized;
        CHECK(1 == initialized.start_count);
        CHECK(1 == initialized.stop_count);
    }

    SECTION("timer scope")
    {
        timer_mock timer;
        CHECK(0 == timer.start_count);
        CHECK(0 == timer.stop_count);
        {
            platform::timer_scope<timer_mock> scope(timer);
            CHECK(1 == timer.start_count);
            CHECK(0 == timer.stop_count);
        }
        CHECK(1 == timer.start_count);
        CHECK(1 == timer.stop_count);
    }

    SECTION("started performance counter")
    {
        platform::performance_initialized_timer timer;
        CHECK(timer.get_microseconds() <= 1);
    }

    SECTION("ordinary timer")
    {
        platform::performance_counter timer;
        {
            platform::performance_scope scope(timer);
            std::this_thread::sleep_for(std::chrono::milliseconds(20));
        }
        CHECK(timer.get_milliseconds() > 15);
        CHECK(timer.get_milliseconds() < 25);
        {
            platform::performance_scope scope(timer);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        CHECK(timer.get_milliseconds() > 5);
        CHECK(timer.get_milliseconds() < 15);
    }

    SECTION("accumulation timer")
    {
        platform::acc_performance_initialized_timer timer;
        {
            platform::acc_performance_scope scope(timer);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        CHECK(timer.get_milliseconds() > 5);
        CHECK(timer.get_milliseconds() < 15);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        {
            platform::acc_performance_scope scope(timer);
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        CHECK(timer.get_milliseconds() > 15);
        CHECK(timer.get_milliseconds() < 25);
    }
}

