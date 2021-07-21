#pragma once
#include <chrono>
#include <functional>
#include <iostream>
#include <utility>

namespace Timers {

/**
 * @brief - Interface class for timer's functionality
 */
class Timer {
protected:
    /**
     * @brief - Priority of this timer, depending on it order of timers execuration is created
     */
    uint32_t priority{};
    /**
     * @brief - Callback function called on timer expiration
     */
    std::function<void()> callback;
    /**
     * @brief - Point of time in which timer started ticking ( default is timer creation time )
     */
    std::chrono::high_resolution_clock::time_point startTimePoint{std::chrono::high_resolution_clock::now()};
    /**
     * @brief - Duration of timer ticking
     */
    std::chrono::high_resolution_clock::duration duration{};

public:
    /**
     * @brief - Actions, which are returned from callback's.
     * @details - NONE ( No actions are required )
     * @details - DELETE ( Remove this timer from cache )
     */
    enum CallbackAction { NONE, DELETE };
    /**
     * @brief - Default timer interface constructor
     */
    Timer() = default;
    /**
     * @brief - Timer interface constructor
     * @param callback - Callback function called on timer expiration
     */
    explicit Timer(std::function<void()> callback);
    /**
     * @brief - Timer interface constructor
     * @param callback - Callback function called on timer expiration
     * @param duration - Duration of timer ticking
     */
    Timer(std::function<void()> callback, std::chrono::high_resolution_clock::duration duration);
    /**
     * @brief - Timer interface constructor
     * @param callback - Callback function called on timer expiration
     * @param expirationTime - Point in time in which timer should expire
     */
    Timer(std::function<void()> callback, std::chrono::high_resolution_clock::time_point expirationTime);
    /**
     * @brief - Restart timer with current time point
     */
    void restart() noexcept;
    /**
     * @brief - Executes callback function
     * @return - Code associated with specific deriving class
     */
    virtual CallbackAction run() = 0;
    /**
     * @brief - set callback function
     * @param callback - Callback function called on timer expiration
     */
    void setCallback(std::function<void()> callback);
    /**
     * @brief - Get time point in which timer will expire
     * @return - Expiration time point
     */
    [[nodiscard]] std::chrono::high_resolution_clock::time_point getExpirationTimePoint() const;
    /**
     * @brief - Set new priority of timer
     * @param priority - new priority of timer
     */
    void setPriority(uint32_t priority) noexcept;
    /**
     * @brief - Get priority of timer
     * @return - priority of timer
     */
    [[nodiscard]] uint32_t getPriority() const;
    /**
     * @brief - Set new expiration time point of timer
     * @param timePoint - new expiration time point
     */
    void setExpirationTimePoint(std::chrono::high_resolution_clock::time_point timePoint);
};

/**
 * @brief - Timer's class which are intended to be called only once
 */
class OneshotTimer : public Timer {
public:
    /**
     * @brief - Default one shot timer constructor
     */
    OneshotTimer() = default;
    /**
     * @brief - One shot timer constructor
     * @param callback - Callback function called on timer expiration
     */
    explicit OneshotTimer(std::function<void()> callback);
    /**
     * @brief - One shot timer constructor
     * @param callback - Callback function called on timer expiration
     * @param duration - Duration of timer ticking
     */
    OneshotTimer(std::function<void()> callback, std::chrono::high_resolution_clock::duration duration);
    /**
     * @brief - One shot timer constructor
     * @param callback - Callback function called on timer expiration
     * @param expirationTime - Point in time in which timer should expire
     */
    OneshotTimer(std::function<void()> callback, std::chrono::high_resolution_clock::time_point expirationTime);
    /**
     * @brief - Executes callback function
     * @return - Always return delete code for one shot timer
     */
    [[nodiscard]] CallbackAction run() override;
};

/**
 * @brief - Timer's class which are intended to be repeatable
 */
class RepeatableTimer : public Timer {
protected:
    /**
     * @brief - Expiration's counter, atomic for thread safety
     */
    std::atomic<uint32_t> expirationCount{};

public:
    RepeatableTimer() = default;
    /**
     * @brief - Repeatable timer constructor
     * @param callback - Callback function called on timer expiration
     */
    explicit RepeatableTimer(std::function<void()> callback);
    /**
     * @brief - Repeatable timer constructor
     * @param callback - Callback function called on timer expiration
     * @param duration - Duration of timer ticking
     */
    RepeatableTimer(std::function<void()> callback, std::chrono::high_resolution_clock::duration duration);
    /**
     * @brief - Repeatable timer constructor
     * @param callback - Callback function called on timer expiration
     * @param expirationTime - Point in time in which timer should expire
     */
    RepeatableTimer(std::function<void()> callback, std::chrono::high_resolution_clock::time_point expirationTime);
    /**
     * @brief - Executes callback function
     * @return - Always return none code for repeatable timer
     */
    [[nodiscard]] CallbackAction run() override;
    /**
     * @brief - Get how many times timer expired
     * @return - timer expiration count
     */
    [[nodiscard]] uint32_t getExpirationsCount() const;
};

} // namespace Timers