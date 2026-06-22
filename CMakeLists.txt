cmake_minimum_required(VERSION 3.15)

project(WebServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Server executable
add_executable(
    server
    src/Server.cpp
    src/Logger.cpp
)

target_link_libraries(
    server
    ws2_32
)

# Client executable
add_executable(
    client
    src/Client.cpp
    src/Logger.cpp
)

target_link_libraries(
    client
    ws2_32
)
