#include "Timers.hpp"
#include <catch2/catch.hpp>
#include <iostream>

constexpr int DEFAULT_TIMER_VALUE = 5;
constexpr int EXPECTED_TIMER_VALUE = 10;

using namespace Timers;

class Callback {
private:
    int& value;

public:
    explicit Callback(int& val) : value{val} {}
    void operator()() { value = EXPECTED_TIMER_VALUE; }
};

TEST_CASE("One shot timer creation test", "[OneShotTimers") {
    TimerPtr timer{nullptr};
    int timerValue = DEFAULT_TIMER_VALUE;

    SECTION("Default") {
        timer = makeOneShotTimer();
        timer->setCallback([&timerValue]() { timerValue = EXPECTED_TIMER_VALUE; });
    }
    SECTION("Lambda callback") {
        timer = makeOneShotTimer([&timerValue]() { timerValue = EXPECTED_TIMER_VALUE; });
    }
    SECTION("Functor callback") { timer = makeOneShotTimer(Callback(timerValue)); }
    SECTION("Callback with expiration time point") {
        auto firstExpiration = std::chrono::high_resolution_clock::now() + std::chrono::seconds(5);

        SECTION("Lambda callback") {
            timer = makeOneShotTimer([&timerValue]() { timerValue = EXPECTED_TIMER_VALUE; }, firstExpiration);
        }
        SECTION("Functor callback") { timer = makeOneShotTimer(Callback(timerValue), firstExpiration); }

        REQUIRE(timer->getExpirationTimePoint() == firstExpiration);
    }

    REQUIRE(timer != nullptr);
    REQUIRE(timerValue == 5);
    REQUIRE(timer->getPriority() == 0);

    timer->run();
    REQUIRE(timerValue == EXPECTED_TIMER_VALUE);

    timer->setPriority(5);
    REQUIRE(timer->getPriority() == 5);

    auto expiration = std::chrono::high_resolution_clock::now() + std::chrono::seconds(5);
    timer->setExpirationTimePoint(expiration);
    REQUIRE(timer->getExpirationTimePoint() == expiration);

    // It will throw as timer manager is not initialized
    REQUIRE_THROWS(timer->start());
}