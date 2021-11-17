#pragma once
#include "Internal/TimersActionsQueue.hpp"
#include "Internal/TimersCache.hpp"
#include "Internal/TimersImplementation.hpp"
#include "Internal/TimersLogger.hpp"
#include "Internal/TimersRunner.hpp"
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
    ThreadControl m_threadControl{};
    /**
     * @brief - Timers container
     */
    TimersCache timersCache{};
    /**
     * @brief - All actions are processed by this module
     */
    ActionsQueue actionsQueue{m_threadControl, timersCache};
    /**
     * @brief - Thread on which timers are ticking
     */
    std::thread timersThread{};
    /**
     * @brief - Logger instance
     */
    std::shared_ptr<Logger> logger{nullptr};

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
     * @brief - Initialize TimersManager Singleton
     */
    static void initialize();
    /**
     * @brief - Check if Timers Manager is initialized
     */
    static bool isInitialized();

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
    static void addTimer(std::shared_ptr<Timer> timer, std::function<void(uint8_t retCode, bool newRunningState)> fnc);
    /**
     * @brief - Remove timer
     * @param timer - timer to erase
     */
    static void eraseTimer(std::shared_ptr<Timer> timer);
    /**
     * @brief - Set logger instance
     */
    static void setLogger(std::shared_ptr<Logger>);
};

} // namespace Timers
