cmake_minimum_required(VERSION 3.24.2)
project(task)
set(CMAKE_CXX_STANDARD 17)
include(FetchContent)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)

include_directories(${CMAKE_SOURCE_DIR}/Core)
include_directories(${CMAKE_SOURCE_DIR}/Test/Unit)

add_executable(${PROJECT_NAME}
    Core/main.cpp
    Core/FastQueue.cpp
    Core/GameTable.cpp
    Core/LogHandler.cpp
    Core/LogParser.cpp
    Core/Time.cpp
    Core/utils.cpp
)

enable_testing()

add_executable(LogParserTests
    Test/Unit/LogParserTest.cpp
    Core/LogParser.cpp
    Core/Time.cpp
    Core/utils.cpp
)
target_link_libraries(LogParserTests
    gtest
    gtest_main
)

add_executable(StoiDecoratorTests
    Test/Unit/StoiDecoratorTest.cpp
    Core/utils.cpp
)
target_link_libraries(StoiDecoratorTests
    gtest
    gtest_main
)

add_executable(TimeTests
    Test/Unit/TimeTest.cpp
    Core/Time.cpp
    Core/utils.cpp
)
target_link_libraries(TimeTests
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(LogParserTests)
gtest_discover_tests(StoiDecoratorTests)
gtest_discover_tests(TimeTests)
