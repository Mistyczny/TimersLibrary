#pragma once
#include "Internal/TimersActionsQueue.hpp"
#include "Internal/TimersCache.hpp"
#include "Internal/TimersImplementation.hpp"
#include "Internal/TimersThreadControl.hpp"
#include <condition_variable>
#include <future>
#include <iostream>
#include <mutex>
#include <thread>
#include <utility>

namespace Timers {

/**
 * @brief - Class responsible for timers thread functionality
 */
class TimersRunner {
private:
    static std::cv_status wait_for_expiration(ThreadControl& control, std::chrono::high_resolution_clock::time_point timePoint) {
        std::unique_lock lock(control.lock);
        return control.cond.wait_until(lock, timePoint);
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

public:
    /**
     * @brief - Functionality responsible for taking timers actions
     * @param running - Control over running of timers thread
     * @param timersCache - Timers container
     * @param control - Timers thread-container synchronization
     */
    void operator()(std::atomic<bool>& running, TimersCache& timersCache, ThreadControl& control, ActionsQueue& actionsQueue) {
        while (running) {
            actionsQueue.process();

            std::cout << "RUNNING" << std::endl;
            auto nextExpirationTimePoint = timersCache.getNextExpirationTimePoint();
            if (nextExpirationTimePoint.has_value()) {
                if (wait_for_expiration(control, *nextExpirationTimePoint) == std::cv_status::timeout) {
                    std::list<std::shared_ptr<Timer>> expiredTimers = timersCache.getTimersExpiringAt(*nextExpirationTimePoint);
                    if (!expiredTimers.empty()) {
                        std::list<std::shared_ptr<Timer>> timersToDelete = executeExpiredTimers(timersCache, expiredTimers);

                        std::for_each(std::begin(timersToDelete), std::end(timersToDelete),
                                      [&](std::shared_ptr<Timer> timerToDelete) { timersCache.deleteTimer(std::move(timerToDelete)); });
                    }
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