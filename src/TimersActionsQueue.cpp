#include "Internal//TimersActionsQueue.hpp"

namespace Timers {

ActionsQueue::ActionsQueue(ThreadControl& timersThreadControl, TimersCache& timersCache)
    : m_timersThreadControl{timersThreadControl}, m_timersCache{timersCache} {}

void ActionsQueue::push(TIMER_ACTION timerAction, std::shared_ptr<Timers::Timer> timer, std::function<void(uint8_t, bool)> callback) {
    std::lock_guard lockGuard{this->m_mutex};
    this->m_actionsQueue.emplace(timerAction, timer, callback);
}

void ActionsQueue::process() {
    std::lock_guard lockGuard{this->m_mutex};
    while (!m_actionsQueue.empty()) {
        try {
            auto& [action, timer, callback] = m_actionsQueue.front();
            switch (action) {
            case START:
                m_timersCache.registerTimer(timer);
                break;
            case STOP:
                m_timersCache.deleteTimer(timer);
                break;
            case RESTART:

                std::cout << "Currently not supported" << std::endl;
                break;
            default:
                std::cout << "Invalid actions" << std::endl;
                break;
            }
            if (callback) {
                callback(action, true);
            }
        } catch (std::exception& ex) {
            std::cout << ex.what() << std::endl;
        }
        m_actionsQueue.pop();
    }
    std::cout << "Processing finished" << std::endl;
}

} // namespace Timers