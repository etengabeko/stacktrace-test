cmake_minimum_required(VERSION 3.1)
project(stacktrace)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENTIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt5Widgets CONFIG REQUIRED)

find_package(Boost 1.65 REQUIRED COMPONENTS
    filesystem
    system
)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

set(SOURCES
    # widgets:
    src/crashreportwidget.cpp
    src/mainwidget.cpp
    # main:
    src/main.cpp
)

set(HEADERS
    # basic:
    src/common.h
    # widgets:
    src/crashreportwidget.h
    src/mainwidget.h
)

qt5_wrap_ui(FORMS
    src/crashreportwidget.ui
    src/mainwidget.ui
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${FORMS})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets ${Boost_LIBRARIES})

if (UNIX)
    target_link_libraries(${PROJECT_NAME} dl backtrace)
    add_definitions(-DBOOST_STACKTRACE_USE_BACKTRACE)
else (UNIX)
    if (MSVC)
        target_link_libraries(${PROJECT_NAME} dbgeng)
        add_definitions(-DBOOST_STACKTRACE_USE_WINDBG)
    elseif (MSVC)
        # FIXME
    endif (MSVC)
    target_link_libraries(${PROJECT_NAME} ole32)
endif(UNIX)

if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wall -Wextra -Werror)
endif (CMAKE_COMPILER_IS_GNUCXX)
