#pragma once
#include <mutex>
#include <condition_variable>

namespace Timers {

/**
 * @brief - Timers thread controll
 */
struct ThreadControl {
    std::mutex lock;
    std::condition_variable cond;
};

}