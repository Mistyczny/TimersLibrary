#include "Internal/TimersImplementation.hpp"
#include "Internal/TimersManager.hpp"

namespace Timers {

/**
 * @brief - Timer interface constructor
 * @param callback - Callback function called on timer expiration
 */
Timer::Timer(std::function<void()> callback) : callback{std::move(callback)} {}

/**
 * @brief - Timer interface constructor
 * @param callback - Callback function called on timer expiration
 * @param duration - Duration of timer ticking
 */
Timer::Timer(std::function<void()> callback, std::chrono::high_resolution_clock::duration duration)
    : callback{std::move(callback)}, duration{duration} {}

/**
 * @brief - Timer interface constructor
 * @param callback - Callback function called on timer expiration
 * @param expirationTime - Point in time in which timer should expire
 */
Timer::Timer(std::function<void()> callback, std::chrono::high_resolution_clock::time_point expirationTime)
    : callback{std::move(callback)} {
    this->startTimePoint = std::chrono::high_resolution_clock::now();
    this->duration = expirationTime - this->startTimePoint;
}

/**
 * @brief - Restart timer with current time point
 */
void Timer::restart() noexcept { this->startTimePoint = std::chrono::high_resolution_clock::now(); }

/**
 * @brief - Get time point in which timer will expire
 * @return - Expiration time point
 */
std::chrono::high_resolution_clock::time_point Timer::getExpirationTimePoint() const { return this->startTimePoint + this->duration; }

/**
 * @brief - set callback function
 * @param callback - Callback function called on timer expiration
 */
void Timer::setCallback(std::function<void()> callback) { this->callback = std::move(callback); }

/**
 * @brief - Set new priority of timer
 * @param priority - new priority of timer
 */
void Timer::setPriority(const uint32_t priority) noexcept { this->priority = priority; }

/**
 * @brief - Get priority of timer
 * @return - priority of timer
 */
uint32_t Timer::getPriority() const { return this->priority; }

/**
 * @brief - Set new expiration time point of timer
 * @param timePoint - new expiration time point
 */
void Timer::setExpirationTimePoint(std::chrono::high_resolution_clock::time_point expirationTimePoint) {
    this->duration = expirationTimePoint - startTimePoint;
}

/**
 * @brief - Set timer running variable
 */
void Timer::setRunningState(const bool& newState) { this->running = newState; }

/**
 * @brief - Send add action to timer's thread, and wait until it's started
 */
void Timer::start() {
    if (this->running) {
        return;
    } else {
        std::promise<int> returnCode{};
        auto future = returnCode.get_future();
        auto timerReadyCallback = [this, &returnCode](int retCode, bool newRunningState) {
            returnCode.set_value(retCode);
            this->setRunningState(newRunningState);
        };
        TimersManager::addTimer(shared_from_this(), timerReadyCallback);
        future.get();
    }
}

/**
 * @brief - Send add action to timer's thread, and continue execution
 */
void Timer::startAsync() {
    if (this->running) {
        return;
    } else {
        TimersManager::addTimer(shared_from_this(), nullptr);
    }
}

/**
 * @brief - Send stop action to timer's thread
 */
void Timer::stop() {
    if (!this->running) {
        return;
    } else {
        TimersManager::eraseTimer(shared_from_this());
        while (this->running) {
            std::this_thread::yield();
        }
    }
}

/**
 * @brief - Get duration
 */
std::chrono::high_resolution_clock::duration Timer::getDuration() const { return this->duration; }

/**
 * @brief - One shot timer constructor
 * @param callback - Callback function called on timer expiration
 */
OneShotTimer::OneShotTimer(std::function<void()> callback) : Timer(std::move(callback)) {}

/**
 * @brief - One shot timer constructor
 * @param callback - Callback function called on timer expiration
 * @param duration - Duration of timer ticking
 */
OneShotTimer::OneShotTimer(std::function<void()> callback, std::chrono::high_resolution_clock::duration duration)
    : Timer(std::move(callback), duration) {}

/**
 * @brief - One shot timer constructor
 * @param callback - Callback function called on timer expiration
 * @param expirationTime - Point in time in which timer should expire
 */
OneShotTimer::OneShotTimer(std::function<void()> callback, std::chrono::high_resolution_clock::time_point expirationTime)
    : Timer(std::move(callback), expirationTime) {}

/**
 * @brief - Executes callback function
 * @return - Always return delete code for one shot timer
 */
Timer::CallbackAction OneShotTimer::run() {
    if (this->callback) {
        this->callback();
    } else {
        std::cout << "Timer callback function is not provided" << std::endl;
    }
    return CallbackAction::DELETE;
}

/**
 * @brief - Repeatable timer constructor
 * @param callback - Callback function called on timer expiration
 */
RepeatableTimer::RepeatableTimer(std::function<void()> callback) : Timer(std::move(callback)) {}

/**
 * @brief - Repeatable timer constructor
 * @param callback - Callback function called on timer expiration
 * @param duration - Duration of timer ticking
 */
RepeatableTimer::RepeatableTimer(std::function<void()> callback, std::chrono::high_resolution_clock::duration duration)
    : Timer(std::move(callback), duration) {}

/**
 * @brief - Repeatable timer constructor
 * @param callback - Callback function called on timer expiration
 * @param expirationTime - Point in time in which timer should expire
 */
RepeatableTimer::RepeatableTimer(std::function<void()> callback, std::chrono::high_resolution_clock::time_point expirationTime)
    : Timer(std::move(callback), expirationTime) {}

/**
 * @brief - Executes callback function
 * @return - Always return none code for repeatable timer
 */
Timer::CallbackAction RepeatableTimer::run() {
    expirationCount++;
    if (this->callback) {
        this->callback();
    }
    return CallbackAction::NONE;
}

uint32_t RepeatableTimer::getExpirationsCount() const { return this->expirationCount; }

} // namespace Timers