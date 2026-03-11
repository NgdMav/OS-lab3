cmake_minimum_required(VERSION 3.14)

project(Lab2_Threads LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Настройка для Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Основной исполняемый файл
add_library(thread_lib STATIC
    src/thread_functions.cpp
    include/thread_functions.h
)

add_executable(lab2_threads
    src/main.cpp
)

target_link_libraries(lab2_threads PRIVATE 
    thread_lib
)

# Включение тестирования
enable_testing()

add_executable(tests
    tests/test_threads.cpp
)
target_include_directories(tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(tests PRIVATE
    thread_lib
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
