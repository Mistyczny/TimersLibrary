#include "TimersManager.hpp"
#include "Error.hpp"
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
 * @return - true, if initialization success, false otherwise
 */
bool TimersManager::initialize() {
    bool initialized{true};
    if (!instance) {
        std::call_once(initInstanceFlag, [&]() {
            try {
                instance = new TimersManager();
            } catch (std::bad_alloc&) {
                initialized = false;
            }
        });
    }
    return initialized;
}

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
    if (initialize()) {
        threadsRunning = getInstance().threadsRunning;
    } else {
        throw InitializationError("Failed to initialize timers manager instance");
    }
    return threadsRunning;
}

/**
 * @brief - Start's timers thread
 */
void TimersManager::start() {
    if (initialize()) {
        TimersManager& timersManager = getInstance();
        if (!timersManager.isRunning()) {
            timersManager.threadsRunning = true;
            timersManager.timersThread = std::thread(TimersRunner(), std::ref(timersManager.threadsRunning),
                                                     std::ref(timersManager.timersCache), std::ref(timersManager.timersThreadControl));
        }
    } else {
        throw InitializationError("Failed to initialize timers manager instance");
    }
}

/**
 * @brief - Stop's timers thread
 */
void TimersManager::stop() {
    if (initialize()) {
        TimersManager& timersManager = getInstance();
        if (timersManager.threadsRunning) {
            timersManager.threadsRunning = false;
            timersManager.timersThreadControl.cond.notify_one();
            timersManager.timersThread.join();
        }
    } else {
        throw InitializationError("Failed to initialize timers manager instance");
    }
}

/**
 * @brief - Register new timer
 * @param timer - timer to register
 */
void TimersManager::addTimer(std::shared_ptr<Timer> timer) {
    if (initialize()) {
        TimersManager& timersManager = getInstance();
        // Register timers in extra scope, to enable notify'ing after lock is destroyed
        {
            std::unique_lock lock{timersManager.timersThreadControl.lock};
            timersManager.timersCache.registerTimer(std::move(timer));
        }
        timersManager.timersThreadControl.cond.notify_one();
    } else {
        throw InitializationError("Failed to initialize timers manager instance");
    }
}

/**
 * @brief - Register new timers
 * @param timers - List of timers to register
 */
void TimersManager::addTimers(std::initializer_list<std::shared_ptr<Timer>> timers) {
    if (initialize()) {
        if (!empty(timers)) {
            TimersManager& timersManager = getInstance();
            // Register timers in extra scope, to enable notify'ing after lock is destroyed
            {
                std::unique_lock lock{timersManager.timersThreadControl.lock};
                std::for_each(std::begin(timers), std::end(timers), [&](auto timer) { timersManager.timersCache.registerTimer(timer); });
            }
            timersManager.timersThreadControl.cond.notify_one();
        }
    } else {
        throw InitializationError("Failed to initialize timers manager instance");
    }
}

} // namespace Timers
