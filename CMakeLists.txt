cmake_minimum_required(VERSION 3.1)
project(stacktrace)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENTIONS OFF)

if (CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-Wall -Wextra -Werror)
endif (CMAKE_COMPILER_IS_GNUCXX)

find_package(Boost 1.65 REQUIRED COMPONENTS
    filesystem
    system
)

include_directories(${Boost_INCLUDE_DIRS})    

set(SOURCES
    main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

if (UNIX)
    target_link_libraries(${PROJECT_NAME} dl)
endif(UNIX)
