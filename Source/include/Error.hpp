#pragma once
#include <exception>

namespace Timers {

class InitializationError : std::exception {
private:
    std::string msg{};

public:
    /**
     * @brief - Constructor ( C String)
     * @param message - C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the char* lies
     *                 with the caller.
     */
    explicit InitializationError(const char* message) : msg(message) {}
    /**
     * @brief - Constructor (C++ STL strings).
     * @param message The error message.
     */
    explicit InitializationError(const std::string& message) : msg(message) {}
    /**
     * @brief - Destructor, Virtual to allow for subclassing.
     */
    ~InitializationError() noexcept override = default;
    /**
     * @brief - Returns a pointer to the (constant) error description.
     * @return - A pointer to a const char*. The underlying memory
     *          is in posession of the Exception object. Callers must
     *          not attempt to free the memory.
     */
    [[nodiscard]] const char* what() const noexcept override { return msg.c_str(); }
};

class TimerError : std::exception {
private:
    std::string msg{};

public:
    /**
     * @brief - Constructor ( C String)
     * @param message - C-style string error message.
     *                 The string contents are copied upon construction.
     *                 Hence, responsibility for deleting the char* lies
     *                 with the caller.
     */
    explicit TimerError(const char* message) : msg(message) {}
    /**
     * @brief - Constructor (C++ STL strings).
     * @param message The error message.
     */
    explicit TimerError(const std::string& message) : msg(message) {}
    /**
     * @brief - Destructor, Virtual to allow for subclassing.
     */
    ~TimerError() noexcept override = default;
    /**
     * @brief - Returns a pointer to the (constant) error description.
     * @return - A pointer to a const char*. The underlying memory
     *          is in posession of the Exception object. Callers must
     *          not attempt to free the memory.
     */
    [[nodiscard]] const char* what() const noexcept override { return msg.c_str(); }
};

} // namespace Timers