#include "Internal/TimersCache.hpp"
#include "Internal/TimersError.hpp"
#include <thread>

namespace Timers {

/**
 * @brief - If timer is not registered yet, add it to container
 * @param timer - Timer to register
 */
void TimersCache::registerTimer(std::shared_ptr<Timer> timer) {
    if (timer && !isTimerRegistered(timer)) {
        m_timers.insert(std::make_pair(timer->getExpirationTimePoint(), timer));
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

        if (m_timers.contains(expirationTimePoint)) {
            auto range = this->m_timers.equal_range(expirationTimePoint);
            for (auto it = range.first; it != range.second; ++it) {
                if (it->second == timer) {
                    m_timers.erase(it);
                    break;
                }
            }
        }
    } else {
        throw TimerError("Timer is not initialized - nullptr");
    }
}

void TimersCache::restartTimer(std::shared_ptr<Timer> timer) {
    if (timer) {
        auto expirationTimePoint = timer->getExpirationTimePoint();

        if (m_timers.contains(expirationTimePoint)) {
            auto range = this->m_timers.equal_range(expirationTimePoint);
            for (auto it = range.first; it != range.second; ++it) {
                if (it->second == timer) {
                    m_timers.erase(it);
                    auto newExpirationTimePoint = std::chrono::high_resolution_clock::now() + timer->getDuration();
                    timer->setExpirationTimePoint(newExpirationTimePoint);
                    m_timers.insert(std::make_pair(timer->getExpirationTimePoint(), timer));
                    break;
                }
            }
        }
    } else {
        throw TimerError("Timer is not initialized - nullptr");
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

        if (m_timers.contains(expirationTimePoint)) {
            auto range = this->m_timers.equal_range(expirationTimePoint);
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
size_t TimersCache::size() const { return this->m_timers.size(); }

/**
 * @brief - Get closest expiration time point of timer registered in container
 * @return - Closest expiration time point
 */
std::optional<std::chrono::high_resolution_clock::time_point> TimersCache::getNextExpirationTimePoint() {
    std::optional<std::chrono::high_resolution_clock::time_point> nextExpiration{std::nullopt};
    if (!m_timers.empty()) {
        nextExpiration = std::begin(this->m_timers)->first;
    }
    return nextExpiration;
}

std::list<std::shared_ptr<Timer>> TimersCache::getTimersExpiringAt(std::chrono::high_resolution_clock::time_point timePoint) {
    std::list<std::shared_ptr<Timer>> expiredTimers{};

    auto range = this->m_timers.equal_range(timePoint);
    for (auto it = range.first; it != range.second; ++it) {
        expiredTimers.emplace_back(it->second);
    }
    expiredTimers.sort(
        [](const std::shared_ptr<Timer> first, std::shared_ptr<Timer> second) { return first->getPriority() > second->getPriority(); });
    return expiredTimers;
}

} // namespace Timers