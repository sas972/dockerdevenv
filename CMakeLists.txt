cmake_minimum_required(VERSION 3.20)
project(asio_tcp_server CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Threads REQUIRED)
find_package(Boost 1.74 REQUIRED)

add_executable(server src/main.cpp)

target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(server PRIVATE
    Boost::boost
    Threads::Threads
)
