#pragma once
#include "Internal/TimersImplementation.hpp"

namespace Timers {

TimerPtr makeOneShotTimer();
TimerPtr makeOneShotTimer(const std::function<void()>&);
TimerPtr makeOneShotTimer(const std::function<void()>&, std::chrono::high_resolution_clock::duration);
TimerPtr makeOneShotTimer(const std::function<void()>&, std::chrono::high_resolution_clock::time_point);

} // namespace Timers