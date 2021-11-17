#include "Timers.hpp"
#include <catch2/catch.hpp>
#include <iostream>

using namespace Timers;

class Callback {
private:
    int& value;

public:
    explicit Callback(int& val) : value{val} {}
    void operator()() { value = 10; }
};

class CallbackTwo {
private:
    int& value;

public:
    explicit CallbackTwo(int& val) : value{val} {}
    void run() { value = 10; }
};

TEST_CASE( "Repeatable timer creation test", "[RepeatableTimers" ) {
    std::shared_ptr<RepeatableTimer> timer{nullptr};
    int timerValue = 5;

    SECTION("Lambda callback") {
        timer = std::make_shared<RepeatableTimer>([&timerValue]() { timerValue = 10; });
    }
    SECTION("Functor callback") { timer = std::make_shared<RepeatableTimer>(Callback(timerValue)); }
    SECTION("Object call") {
        CallbackTwo callbackTwo{timerValue};
        timer = std::make_shared<RepeatableTimer>(std::bind(&CallbackTwo::run, callbackTwo));
    }
    REQUIRE(timer != nullptr);

    // Validate that expiration count is set to 0
    REQUIRE(timer->getExpirationsCount() == 0);

    // Validate if callback function works correctly
    timer->run();
    REQUIRE(timerValue == 10);

    // Priority setting
    timer->setPriority(5);
    REQUIRE(timer->getPriority() == 5);

    // Expiration set
    std::chrono::high_resolution_clock::time_point expiration = std::chrono::high_resolution_clock::now() + std::chrono::seconds(5);
    timer->setExpirationTimePoint(expiration);
    REQUIRE(timer->getExpirationTimePoint() == expiration);

}