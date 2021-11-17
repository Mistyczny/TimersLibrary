#pragma once
#include "Internal/TimersCache.hpp"
#include "Internal/TimersImplementation.hpp"
#include "Internal/TimersThreadControl.hpp"
#include <memory>
#include <queue>
#include <utility>

namespace Timers {

enum TIMER_ACTION { START, STOP, RESTART };

typedef std::pair<TIMER_ACTION, std::shared_ptr<Timers::Timer>> TimerAction;

struct Action {
    TIMER_ACTION m_action;
    std::shared_ptr<Timers::Timer> m_timer;
    std::function<void(uint8_t retCode, bool newRunningState)> m_callback;
    Action(TIMER_ACTION action, std::shared_ptr<Timers::Timer> timer, std::function<void(uint8_t retCode, bool newRunningState)> callback)
        : m_action{action}, m_timer{std::move(std::move(timer))}, m_callback{std::move(callback)} {}
};

class ActionsQueue {
private:
    std::mutex m_mutex;
    ThreadControl& m_timersThreadControl;
    TimersCache& m_timersCache;
    std::queue<Action> m_actionsQueue{};

public:
    explicit ActionsQueue(ThreadControl& timersThreadControl, TimersCache& timersCache);
    void push(TIMER_ACTION, std::shared_ptr<Timers::Timer>, std::function<void(uint8_t retCode, bool newRunningState)>);
    void process();
};

} // namespace Timers