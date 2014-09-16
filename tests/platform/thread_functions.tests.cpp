#include <catch/catch.hpp>
#include <platform/thread_functions.h>
#include <thread>

TEST_CASE("get_current_thread_id", "[thread][platform]")
{
    auto tid = platform::current_thread_id();

    platform::thread_id_t childTid = 0;
    std::thread child = std::thread([&childTid]() { childTid = platform::current_thread_id(); });
    child.join();

    std::cout << "tid      =" << std::hex << tid << std::endl;
    std::cout << "child tid=" << std::hex << childTid << std::endl;

    REQUIRE(childTid != tid);
}
