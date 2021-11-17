#include "Timers.hpp"
#include "TestLogger.hpp"
#include <catch2/catch.hpp>
#include <iostream>

using namespace Timers;

TEST_CASE( "TimersManager initialization test", "[TimersManager" ) {
    TimersManager::initialize();
    REQUIRE_THROWS(TimersManager::initialize());

    auto logger = std::make_shared<TestLogger>();
    TimersManager::setLogger(logger);

    REQUIRE(TimersManager::isRunning() == false);
    TimersManager::start();
    REQUIRE(TimersManager::isRunning() == true);

    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();

    int value = 5;
    std::shared_ptr<Timer> timer_1 = std::make_shared<OneShotTimer>([&value]() {
        value = 7;
    });
    timer_1->setExpirationTimePoint(now + std::chrono::seconds(5));
    timer_1->startAsync();
    //TimersManager::addTimer(timer_1);
    std::this_thread::sleep_for(std::chrono::seconds(6));
    REQUIRE(value == 7);

    TimersManager::stop();
    REQUIRE(TimersManager::isRunning() == false);
}