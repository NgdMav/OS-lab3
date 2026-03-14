cmake_minimum_required(VERSION 3.14)

project(Lab3_Synchronization LANGUAGES CXX)

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
add_library(marker_lib STATIC
    src/marker_thread.cpp
    include/marker_thread.h
)

add_executable(lab3_synchronization
    src/main.cpp
)

target_link_libraries(lab3_synchronization PRIVATE 
    marker_lib
)

# Включение тестирования
enable_testing()

add_executable(tests
    tests/test.cpp
)
target_include_directories(tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(tests PRIVATE
    marker_lib
    gtest
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(tests)
