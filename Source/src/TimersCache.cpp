#include "TimersCache.hpp"
#include <thread>
#include "Error.hpp"

namespace Timers {

/**
 * @brief - If timer is not registered yet, add it to container
 * @param timer - Timer to register
 */
void TimersCache::registerTimer(std::shared_ptr<Timer> timer) {
    if (timer && !isTimerRegistered(timer)) {
        timers.insert(std::make_pair(timer->getExpirationTimePoint(), timer));
    } else {
        throw TimerError("Timer registration failure");
    }
}

/**
 * @brief - Delete timer from container
 * @param timer - Timer to delete
 */
void TimersCache::deleteTimer(std::shared_ptr<Timer> timer) {
    if (timer) {
        auto expirationTimePoint = timer->getExpirationTimePoint();

        if (timers.contains(expirationTimePoint)) {
            auto range = this->timers.equal_range(expirationTimePoint);
            for (auto it = range.first; it != range.second; ++it) {
                if (it->second == timer) {
                    timers.erase(it);
                    break;
                }
            }
        }
    } else {
        throw TimerError("Timer is null");
    }
}

/**
 * @brief - Check if timer is registered
 * @param timer - Timer to check
 * @return - true if it is registered, false otherwise
 */
bool TimersCache::isTimerRegistered(std::shared_ptr<Timer> timer) {
    auto alreadyRegistered{false};
    if (timer) {
        auto expirationTimePoint = timer->getExpirationTimePoint();

        if (timers.contains(expirationTimePoint)) {
            auto range = this->timers.equal_range(expirationTimePoint);
            for (auto it = range.first; it != range.second; ++it) {
                if (it->second == timer) {
                    alreadyRegistered = true;
                    break;
                }
            }
        }
    } else {
        throw TimerError("Timer is null");
    }
    return alreadyRegistered;
}

/**
 * @brief - Get number of registered timers in container
 * @return - number of timers
 */
size_t TimersCache::size() const { return this->timers.size(); }

/**
 * @brief - Get closest expiration time point of timer registered in container
 * @return - Closest expiration time point
 */
std::optional<std::chrono::high_resolution_clock::time_point> TimersCache::getNextExpirationTimePoint() {
    std::optional<std::chrono::high_resolution_clock::time_point> nextExpiration{std::nullopt};
    if (!timers.empty()) {
        nextExpiration = std::begin(this->timers)->first;
    }
    return nextExpiration;
}

std::list<std::shared_ptr<Timer>> TimersCache::getTimersExpiringAt(std::chrono::high_resolution_clock::time_point timePoint) {
    std::list<std::shared_ptr<Timer>> expiredTimers{};

    auto range = this->timers.equal_range(timePoint);
    for (auto it = range.first; it != range.second; ++it) {
        expiredTimers.emplace_back(it->second);
    }
    expiredTimers.sort(
        [](const std::shared_ptr<Timer> first, std::shared_ptr<Timer> second) { return first->getPriority() > second->getPriority(); });
    return expiredTimers;
}

} // namespace Timers