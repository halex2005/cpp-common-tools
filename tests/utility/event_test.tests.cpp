#include <catch/catch.hpp>
#include "utility/event_test.h"

SCENARIO("event_test<int> tests")
{
    GIVEN("event_test<int> created")
    {
        utility::event_test<int> eventTest;
        WHEN("eventTest has no event subscriptions")
        {
            THEN("checkFired() should return false")
            {
                CHECK(false == eventTest.checkFired());
            }
        }
        WHEN("eventTest has one event subscription")
        {
            eventTest.eventSubscription(10);
            THEN("checkFired() should return true")
            {
                CHECK(true == eventTest.checkFired());
            }
            THEN("countFired() should return 1")
            {
                CHECK(1 == eventTest.countFired());
            }
            THEN("argument should be 10")
            {
                CHECK(10 == eventTest.getArg(0));
            }
        }
        WHEN("event subscription is asyncronous")
        {
            std::thread thread([&eventTest]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                eventTest.eventSubscription(15);
            });
            THEN("checkFired() should return true")
            {
                CHECK(true == eventTest.checkFired());
            }
            WHEN("checkFired() return true")
            {
                REQUIRE(true == eventTest.checkFired());
                THEN("countFired() should return 1")
                {
                    CHECK(1 == eventTest.countFired());
                }
                THEN("argument should be 15")
                {
                    CHECK(15 == eventTest.getArg(0));
                }
            }
            thread.join();
        }
    }
}

SCENARIO("event_test<void> tests")
{
    GIVEN("event_test<void> created")
    {
        utility::event_test<void> eventTest;
        WHEN("eventTest has no event subscriptions")
        {
            THEN("checkFired() should return false")
            {
                CHECK(false == eventTest.checkFired());
            }
        }
        WHEN("eventTest has one event subscription")
        {
            eventTest.eventSubscription();
            THEN("checkFired() should return true")
            {
                CHECK(true == eventTest.checkFired());
            }
            THEN("countFired() should return 1")
            {
                CHECK(1 == eventTest.countFired());
            }
        }
        WHEN("event subscription is asyncronous")
        {
            std::thread thread([&eventTest]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                eventTest.eventSubscription();
            });
            THEN("checkFired() should return true")
            {
                CHECK(true == eventTest.checkFired());
            }
            WHEN("checkFired() return true")
            {
                REQUIRE(true == eventTest.checkFired());
                THEN("countFired() should return 1")
                {
                    CHECK(1 == eventTest.countFired());
                }
            }
            thread.join();
        }
    }
}