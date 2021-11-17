#pragma once

namespace Timers {

class Logger {
public:
    enum class Level { Critical, Error, Warning, Info, Debug, Trace };

    static std::string levelToString(const Level& level) {
        std::string levelAsString{};
        switch (level) {
        case Level::Critical:
            levelAsString = "Critical";
            break;
        case Level::Error:
            levelAsString = "Error";
            break;
        case Level::Warning:
            levelAsString = "Warning";
            break;
        case Level::Info:
            levelAsString = "Info";
            break;
        case Level::Debug:
            levelAsString = "Debug";
            break;
        case Level::Trace:
            levelAsString = "Trace";
            break;
        }
        return levelAsString;
    }
protected:
    Logger() = default;
    ~Logger() = default;
    virtual void operator()(Level level, std::string_view message) noexcept = 0;
};

} // namespace Timers
