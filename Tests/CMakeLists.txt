enable_testing()
Include(FetchContent)

FetchContent_Declare(
	Catch2
	GIT_REPOSITORY https://github.com/catchorg/Catch2.git
	GIT_TAG        v2.13.1
)

FetchContent_MakeAvailable(Catch2)

add_library(CatchMain STATIC catchMain.cpp)
target_link_libraries(CatchMain
		PUBLIC
	Catch2::Catch2
	Timers
)
target_include_directories(CatchMain
		PUBLIC
	${CMAKE_BINARY_DIR}/include
)

add_executable(OneshotTimerTests ${CMAKE_CURRENT_SOURCE_DIR}/OneshotTimerTests.cpp)
target_link_libraries(OneshotTimerTests
		PUBLIC
	CatchMain
)

add_executable(RepeatableTimerTests ${CMAKE_CURRENT_SOURCE_DIR}/RepeatableTimerTests.cpp)
target_link_libraries(RepeatableTimerTests
		PUBLIC
	CatchMain
)

add_executable(TimersManagerTests ${CMAKE_CURRENT_SOURCE_DIR}/TimersManagerTests.cpp)
target_link_libraries(TimersManagerTests
		PUBLIC
	CatchMain
)

add_executable(TimersCacheTests ${CMAKE_CURRENT_SOURCE_DIR}/TimersCacheTests.cpp)
target_link_libraries(TimersCacheTests
		PUBLIC
	CatchMain
)

add_test(NAME OneshotTimerTests COMMAND  OneshotTimerTests)
add_test(NAME RepeatableTimerTests COMMAND  RepeatableTimerTests)
add_test(NAME TimersManagerTests COMMAND  TimersManagerTests)
add_test(NAME TimersCacheTests COMMAND  TimersCacheTests)