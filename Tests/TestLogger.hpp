#pragma once
#include "Timers.hpp"

class TestLogger : public Timers::Logger {
    void operator()(Level level, std::string_view message) noexcept override {
        std::cout << levelToString(level) << " " << message << std::endl;
    }
};