cmake_minimum_required(VERSION 3.14)
project(LockFree LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

add_executable(TestStack test/TestStack.cpp)
add_executable(TestQueue test/TestQueue.cpp)
