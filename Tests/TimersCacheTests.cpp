#include "Timers.hpp"
#include <catch2/catch.hpp>
#include <chrono>
#include <iostream>

using namespace Timers;

class Callback {
private:
public:
    void operator()() {}
};

TEST_CASE("TimersCache test", "[TimersCache") {
    TimersCache timersCache{};

    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

    std::shared_ptr<Timer> timer_1 = std::make_shared<OneShotTimer>(Callback());
    timer_1->setExpirationTimePoint(now);
    timer_1->setPriority(1);

    std::shared_ptr<Timer> timer_2 = std::make_shared<RepeatableTimer>([]() {});
    timer_2->setExpirationTimePoint(now);
    timer_2->setPriority(2);

    REQUIRE(timersCache.size() == 0);
    timersCache.registerTimer(timer_1);
    REQUIRE_THROWS(timersCache.registerTimer(timer_1));
    timersCache.registerTimer(timer_2);
    REQUIRE(timersCache.size() == 2);

    auto nextExpirationTimestamp = timersCache.getNextExpirationTimePoint();
    if (nextExpirationTimestamp.has_value()) {
        auto expiredTimers = timersCache.getTimersExpiringAt(*nextExpirationTimestamp);
        REQUIRE(expiredTimers.size() == 2);
    }

    timersCache.deleteTimer(timer_1);
    REQUIRE(timersCache.size() == 1);
    timersCache.deleteTimer(timer_2);
    REQUIRE(timersCache.size() == 0);
}