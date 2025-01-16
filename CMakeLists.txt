cmake_minimum_required(VERSION 3.24.2)
project(task)

set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_SOURCE_DIR}/Core)

add_executable(${PROJECT_NAME}
    Core/main.cpp
    Core/FastQueue.cpp
    Core/GameTable.cpp
    Core/LogHandler.cpp
    Core/LogParser.cpp
    Core/Time.cpp
    Core/utils.cpp
)