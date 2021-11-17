#include "Internal/TimersManager.hpp"
#include "Internal/TimersError.hpp"
#include <utility>

namespace Timers {

/**
 * @brief - Singleton instance of TimersManager
 */
TimersManager* TimersManager::instance = nullptr;

/**
 * @brief - Singleton instance of initializer flag
 */
std::once_flag TimersManager::initInstanceFlag{};

/**
 * @brief - Initialize TimersManager Singlegon
 */
void TimersManager::initialize() {
    if (!instance) {
        std::call_once(initInstanceFlag, [&]() {
            try {
                instance = new TimersManager();
            } catch (std::bad_alloc& ex) {
                throw TimersManagerInitializationError(std::string("Memory allocation error - ") + std::string(ex.what()));
            }
        });
    } else {
        throw TimersManagerInitializationError("TimersManager is already initialized");
    }
}

/**
 * @brief - Check if Timers Manager is initialized
 */
bool TimersManager::isInitialized() { return instance; }

/**
 * @brief - Destructor of TimersManager
 */
TimersManager::~TimersManager() { delete instance; }

/**
 * @brief - Get instance of TimersManager Singleton
 * @return - instance of TimersManager
 */
TimersManager& TimersManager::getInstance() { return *instance; }

/**
 * @brief - Returns if timers thread is running
 * @return - true, if timers thread is running, false otherwise
 */
bool TimersManager::isRunning() {
    uint16_t threadsRunning{};
    if (isInitialized()) {
        threadsRunning = getInstance().threadsRunning;
    } else {
        throw TimersManagerError("Timers manager is not initialized");
    }
    return threadsRunning;
}

/**
 * @brief - Start's timers thread
 */
void TimersManager::start() {
    if (isInitialized()) {
        TimersManager& timersManager = getInstance();
        if (!timersManager.isRunning()) {
            timersManager.threadsRunning = true;
            timersManager.timersThread =
                std::thread(TimersRunner(), std::ref(timersManager.threadsRunning), std::ref(timersManager.timersCache),
                            std::ref(timersManager.m_threadControl), std::ref(timersManager.actionsQueue));
        }
    } else {
        throw TimersManagerError("Timers manager is not initialized");
    }
}

/**
 * @brief - Stop's timers thread
 */
void TimersManager::stop() {
    if (isInitialized()) {
        TimersManager& timersManager = getInstance();
        if (timersManager.threadsRunning) {
            timersManager.threadsRunning = false;
            timersManager.m_threadControl.cond.notify_one();
            timersManager.timersThread.join();
        }
    } else {
        throw TimersManagerError("Timers manager is not initialized");
    }
}

/**
 * @brief - Register new timer
 * @param timer - timer to register
 */
void TimersManager::addTimer(std::shared_ptr<Timer> timer, std::function<void(uint8_t retCode, bool newRunningState)> callback) {
    if (isInitialized()) {
        TimersManager& timersManager = getInstance();
        timersManager.actionsQueue.push(TIMER_ACTION::START, std::move(timer), std::move(callback));
        timersManager.m_threadControl.cond.notify_one();
    } else {
        throw TimersManagerError("Timers manager is not initialized");
    }
}

/**
 * @brief - Remove timer
 * @param timer - timer to erase
 */
void TimersManager::eraseTimer(std::shared_ptr<Timer> timer) {
    if (isInitialized()) {
        TimersManager& timersManager = getInstance();
        timersManager.actionsQueue.push(TIMER_ACTION::STOP, std::move(timer), nullptr);
        timersManager.m_threadControl.cond.notify_one();
    } else {
        throw TimersManagerError("Timers manager is not initialized");
    }
}

/**
 * @brief - Set logger instance
 */
void TimersManager::setLogger(std::shared_ptr<Logger> logger) {
    if (isInitialized()) {
        TimersManager& timersManager = getInstance();
        timersManager.logger = logger;
    } else {
        throw TimersManagerError("Timers manager is not initialized");
    }
}

} // namespace Timers
