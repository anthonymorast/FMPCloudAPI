#include "Global.hpp"
#include <chrono>
#include <thread>

auto api = getAPIObject();
TEST_CASE("Caching")
{
    SECTION("Clear Cache")
    {
        // NOTE: verified works with output statements
        auto results = api.getHistoricalChart("INTC", ONE_HOUR);
        std::cout << "Before sleep" << std::endl;
        results = api.getHistoricalChart("INTC", ONE_HOUR);
        auto x = std::chrono::steady_clock::now() + std::chrono::milliseconds(40000);
        std::this_thread::sleep_until(x);
        std::cout << "After sleep" << std::endl;
        results = api.getHistoricalChart("INTC", ONE_HOUR);
    }
}