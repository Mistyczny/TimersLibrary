#pragma once
#include "Timers.hpp"
#include <map>
#include <thread>

namespace Timers {

/**
 * @brief - Container storing all registered timers
 */
class TimersCache {
private:
    /**
     * @brief Multimap in which expiration time point is key
     */
    std::multimap<std::chrono::high_resolution_clock::time_point, std::shared_ptr<Timer>> timers;

public:
    /**
     * @brief - If timer is not registered yet, add it to container
     * @param timer - Timer to register
     */
    void registerTimer(std::shared_ptr<Timer> timer);
    /**
     * @brief - Delete timer from container
     * @param timer - Timer to delete
     */
    void deleteTimer(std::shared_ptr<Timer> timer);
    /**
     * @brief - Check if timer is registered
     * @param timer - Timer to check
     * @return - true if it is registered, false otherwise
     */
    bool isTimerRegistered(std::shared_ptr<Timer> timer);
    /**
     * @brief - Get number of registered timers in container
     * @return - number of timers
     */
    [[nodiscard]] size_t size() const;
    /**
     * @brief - Get closest expiration time point of timer registered in container
     * @return - Closest expiration time point
     */
    [[nodiscard]] std::optional<std::chrono::high_resolution_clock::time_point> getNextExpirationTimePoint();
    /**
     * @brief - Get all expired timers, in specified point of time
     * @param timePoint - Time point
     * @return - All timers from specified point of time
     */
    [[nodiscard]] std::list<std::shared_ptr<Timer>> getTimersExpiringAt(std::chrono::high_resolution_clock::time_point timePoint);
};

}

