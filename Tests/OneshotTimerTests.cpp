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
    explicit CallbackTwo(int& val) : value{val} {
        std::cout << "CREATED" << std::endl;
    }
    void run() { value = 10; }
};

TEST_CASE("Oneshot timer creation test", "[OneshotTimers") {
    std::shared_ptr<Timer> timer{nullptr};
    int timerValue = 5;

    SECTION("Default") {
        timer = std::make_shared<OneshotTimer>();
    }

    SECTION("Lambda callback") {
        timer = std::make_shared<OneshotTimer>([&timerValue]() { timerValue = 10; });
    }
    SECTION("Functor callback") { timer = std::make_shared<OneshotTimer>(Callback(timerValue)); }
    SECTION("Object call") {
        CallbackTwo callbackTwo{timerValue};
        timer = std::make_shared<OneshotTimer>(std::bind(&CallbackTwo::run, callbackTwo));
    }

    SECTION("Callback with expiration time point") {
        std::chrono::high_resolution_clock::time_point firstExpiration = std::chrono::high_resolution_clock::now() + std::chrono::seconds(5);
    }

    REQUIRE(timer != nullptr);

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