#pragma once
#include "Timers.hpp"
#include "TimersCache.hpp"
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <utility>

namespace Timers {

/**
 * @brief - Timers thread controll
 */
struct TimersThreadControl {
    std::mutex lock;
    std::condition_variable cond;
};

/**
 * @brief - Class responsible for timers thread functionality
 */
class TimersRunner {
private:
    /**
     * @brief - Wait for next timers expiration
     * @param control - Timers thread-container synchronization
     * @param timersCache - Timers container
     * @param expirationTimePoint - Point in time, in which next timer expire
     * @return - List of expired timers
     */
    static std::list<std::shared_ptr<Timer>>
    waitForTimersExpiration(TimersThreadControl& control, TimersCache& timersCache,
                            std::chrono::time_point<std::chrono::steady_clock>& expirationTimePoint) {
        std::unique_lock lock(control.lock);
        control.cond.wait_until(lock, expirationTimePoint);

        std::list<std::shared_ptr<Timer>> expiredTimers{};
        // Get timers only if we reach specified point of time
        if (std::chrono::high_resolution_clock::now() >= expirationTimePoint) {
            expiredTimers = timersCache.getTimersExpiringAt(expirationTimePoint);
        }
        return expiredTimers;
    }
    /**
     * @brief - Execute all expired timers
     * @param timersCache - Timers container
     * @param expiredTimers - List of expired timers
     * @return - List of timers to delete from container
     */
    static std::list<std::shared_ptr<Timer>> executeExpiredTimers(TimersCache& timersCache,
                                                                  std::list<std::shared_ptr<Timer>> expiredTimers) {
        std::list<std::shared_ptr<Timer>> timersToDelete{};
        std::for_each(std::begin(expiredTimers), std::end(expiredTimers), [&](std::shared_ptr<Timer> expiredTimer) {
            auto return_code = expiredTimer->run();
            switch (return_code) {
            case Timer::CallbackAction::NONE:
                break;
            case Timer::CallbackAction::DELETE:
                timersToDelete.emplace_back(expiredTimer);
                break;
            default:
                break;
            }
        });

        return timersToDelete;
    }
    /**
     * @brief - Delete all expired one shot timers
     * @param control - Timers thread-container synchronization
     * @param timersCache - Timers container
     * @param timersToDelete - list of timers to delete
     */
    static void deleteExpiredOneshotTimers(TimersThreadControl& control, TimersCache& timersCache,
                                           std::list<std::shared_ptr<Timer>>& timersToDelete) {
        std::unique_lock lock(control.lock);
        std::for_each(std::begin(timersToDelete), std::end(timersToDelete),
                      [&](std::shared_ptr<Timer> timerToDelete) { timersCache.deleteTimer(std::move(timerToDelete)); });
    }

public:
    /**
     * @brief - Functionality responsible for taking timers actions
     * @param running - Control over running of timers thread
     * @param timersCache - Timers container
     * @param control - Timers thread-container synchronization
     */
    void operator()(std::atomic<bool>& running, TimersCache& timersCache, TimersThreadControl& control) {
        while (running) {
            auto nextExpirationTimePoint = timersCache.getNextExpirationTimePoint();
            if (nextExpirationTimePoint.has_value()) {
                // Wait for timers with shortest expiration time
                std::list<std::shared_ptr<Timer>> expiredTimers = waitForTimersExpiration(control, timersCache, *nextExpirationTimePoint);

                // Trigger expired timers
                if (!expiredTimers.empty()) {
                    std::list<std::shared_ptr<Timer>> timersToDelete = executeExpiredTimers(timersCache, expiredTimers);
                    deleteExpiredOneshotTimers(control, timersCache, timersToDelete);
                }
            } else {
                // There are no timers ticking, just wait for new timers
                std::unique_lock lock(control.lock);
                control.cond.wait(lock);
            }
        }
    }
};

} // namespace Timers