
set(SOURCES
	${CMAKE_CURRENT_SOURCE_DIR}/src/Timers.cpp
${CMAKE_CURRENT_SOURCE_DIR}/src/TimersCache.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/src/TimersManager.cpp
)

set(TIMERS_PUBLIC_HEADERS
	"
	${CMAKE_CURRENT_SOURCE_DIR}/include/TimersManager.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/TimersCache.hpp
	${CMAKE_CURRENT_SOURCE_DIR}/include/Timers.hpp
	"
)

add_library(TimersShared SHARED ${SOURCES})

target_include_directories(TimersShared
		PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(TimersShared
		PROPERTIES PUBLIC_HEADER
	${TIMERS_PUBLIC_HEADERS}
)

add_library(TimersStatic STATIC ${SOURCES})

target_include_directories(TimersStatic
		PUBLIC
	${CMAKE_CURRENT_SOURCE_DIR}/include
)


set_target_properties(TimersStatic
		PROPERTIES PUBLIC_HEADER
	${TIMERS_PUBLIC_HEADERS}
)