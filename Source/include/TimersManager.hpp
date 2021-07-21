#pragma once
#include "Timers.hpp"
#include "TimersCache.hpp"
#include "TimersRunner.hpp"
#include <condition_variable>
#include <mutex>
#include <thread>
#include <vector>

namespace Timers {

/**
 * @brief - Manager responsible for managing timers
 */
class TimersManager {
private:
    /**
     * @brief - Singleton instance of TimersManager
     */
    static TimersManager* instance;
    /**
     * @brief - Singleton instance of initializer flag
     */
    static std::once_flag initInstanceFlag;
    /**
     * @brief - Default constructor of TimersManager
     */
    TimersManager() = default;
    /**
     * @brief - Destructor of TimersManager
     */
    ~TimersManager();
    /**
     * @brief - Get instance of TimersManager Singleton
     * @return - instance of TimersManager
     */
    static TimersManager& getInstance();
    /**
     * @brief - Defines, if timers thread is running
     */
    std::atomic<bool> threadsRunning{false};
    /**
     * @brief - Timers thread controller
     */
    TimersThreadControl timersThreadControl{};
    /**
     * @brief - Timers container
     */
    TimersCache timersCache{};
    /**
     * @brief - Thread on which timers are ticking
     */
    std::thread timersThread{};

public:
    /**
     * @brief - Copy constructor is deleted
     */
    TimersManager(const TimersManager&) = delete;
    /**
     * @brief - Copy assign operator is deleted
     */
    TimersManager& operator=(const TimersManager&) = delete;
    /**
     * @brief - Initialize TimersManager Singlegon
     * @return - true, if initialization success, false otherwise
     */
    static bool initialize();
    /**
     * @brief - Returns if timers thread is running
     * @return - true, if timers thread is running, false otherwise
     */
    static bool isRunning();
    /**
     * @brief - Start's timers thread
     */
    static void start();
    /**
     * @brief - Stop's timers thread
     */
    static void stop();
    /**
     * @brief - Register new timer
     * @param timer - timer to register
     */
    static void addTimer(std::shared_ptr<Timer> timer);
    /**
     * @brief - Register new timers
     * @param timers - List of timers to register
     */
    static void addTimers(std::initializer_list<std::shared_ptr<Timer>> timers);
    /**
     * @brief - Remove timer
     * @param timer - timer to remove
     */
    static void eraseTimer(std::shared_ptr<Timer> timer);
};

} // namespace Timers
