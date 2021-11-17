#include "Internal/TimersImplementation.hpp"

namespace Timers {

TimerPtr makeOneShotTimer() { return std::make_shared<OneShotTimer>(); }
TimerPtr makeOneShotTimer(const std::function<void()>& callback) { return std::make_shared<OneShotTimer>(callback); }
TimerPtr makeOneShotTimer(const std::function<void()>& callback, std::chrono::high_resolution_clock::duration duration) {
    return std::make_shared<OneShotTimer>(callback, duration);
}
TimerPtr makeOneShotTimer(const std::function<void()>& callback, std::chrono::high_resolution_clock::time_point expirationTimePoint) {
    return std::make_shared<OneShotTimer>(callback, expirationTimePoint);
}

} // namespace Timers