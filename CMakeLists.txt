cmake_minimum_required(VERSION 3.21)
project(mailbox)

set(CMAKE_CXX_STANDARD 23)

add_executable(test test.cpp server/server_sock.cpp server/server_sock.h
        client/client_sock.cpp client/client_sock.h
        socket.h socket.cpp)
target_link_libraries(test wsock32 ws2_32)
