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
    Test/Unit/main.cpp
    Core/LogParser.cpp
    Core/Time.cpp
    Core/utils.cpp
)
target_link_libraries(LogParserTests
    gtest
)

add_executable(StoiDecoratorTests
    Test/Unit/StoiDecoratorTest.cpp
    Test/Unit/main.cpp
    Core/utils.cpp
)
target_link_libraries(StoiDecoratorTests
    gtest
)

include(GoogleTest)
gtest_discover_tests(LogParserTests)
gtest_discover_tests(StoiDecoratorTests)