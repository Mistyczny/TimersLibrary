cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(TimersLibrary LANGUAGES CXX VERSION 1.0.0)

SET(CMAKE_CXX_STANDARD 20)

option(BUILD_TESTS "Build tests" ON)
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)

set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_PATH ${CMAKE_SOURCE_DIR}/include)

set(SOURCES
    ${SOURCE_PATH}/TimersImplementation.cpp
    ${SOURCE_PATH}/TimersHelpers.cpp
    ${SOURCE_PATH}/TimersCache.cpp
    ${SOURCE_PATH}/TimersManager.cpp
    ${SOURCE_PATH}/TimersActionsQueue.cpp
)

configure_file(${INCLUDE_PATH}/TimersVersion.hpp.in ${CMAKE_BINARY_DIR}/include/TimersVersion.hpp)
set(TIMERS_PUBLIC_HEADERS
    ${INCLUDE_PATH}/Timers.hpp
    ${CMAKE_BINARY_DIR}/include/TimersVersion.hpp
)

set(TIMERS_PRIVATE_HEADER
    ${INCLUDE_PATH}/Internal/TimersManager.hpp
    ${INCLUDE_PATH}/Internal/Error.hpp
    ${INCLUDE_PATH}/Internal/TimersHelpers.hpp
    ${INCLUDE_PATH}/Internal/TimersImplementation.hpp
)

add_library(Timers ${SOURCES})
set_target_properties(Timers PROPERTIES PUBLIC_HEADER "${TIMERS_PUBLIC_HEADERS}")
set_target_properties(Timers PROPERTIES PRIVATE_HEADER "${TIMERS_PRIVATE_HEADER}")
set_target_properties(Timers PROPERTIES POSITION_INDEPENDENT_CODE ${BUILD_SHARED_LIBS})
set_target_properties(Timers PROPERTIES
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
)
target_include_directories(Timers
        PUBLIC
    ${INCLUDE_PATH}
        PRIVATE
    ${CMAKE_BINARY_DIR}/include
)
if(UNIX)
    target_link_libraries(Timers PUBLIC pthread)
endif()

set(CPACK_PACKAGE_NAME "Timers")
set(CPACK_PACKAGE_VENDOR "Kacper Waśniowski")
set(CPACK_PACKAGE_CONTACT "kacp.was@gmail.com")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Timers library")
set(CPACK_PACKAGE_FILE_NAME ${CPACK_PACKAGE_NAME}-${PROJECT_VERSION})

include(GNUInstallDirs)
install(TARGETS Timers
    EXPORT Timers
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT   ${PROJECT_NAME}_RunTime
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT   ${PROJECT_NAME}_RunTime
            NAMELINK_COMPONENT ${PROJECT_NAME}_Development
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT   ${PROJECT_NAME}_Development
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Timers
    PRIVATE_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/Timers/Internal
)

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    ${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

include(CPack)

if(BUILD_TESTS)
    add_subdirectory(Tests)
endif(BUILD_TESTS)