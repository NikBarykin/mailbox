cmake_minimum_required(VERSION 3.21)
project(mailbox)

set(CMAKE_CXX_STANDARD 23)


add_executable(server server/source/main.cpp server/source/server_sock.h
        socket.h socket.cpp server/source/server_sock.cpp server/source/database.cpp server/source/database.h query.cpp query.h answer.cpp answer.h server/source/server.cpp server/source/server.h protocol.cpp protocol.h server/test/test_database.cpp server/test/test_database.h server/source/query_processor.cpp server/source/query_processor.h server/test/test_query_processor.cpp server/test/test_query_processor.h server/test/test.cpp server/test/test_server.cpp server/test/test_server.h)
target_link_libraries(server wsock32 ws2_32)

add_executable(client client/source/main.cpp client/source/client_sock.h
        socket.h socket.cpp
        client/source/client_sock.cpp client/source/interaction.cpp client/source/interaction.h query.cpp query.h answer.cpp answer.h protocol.cpp protocol.h client/source/process_answer.cpp client/source/process_answer.h client/source/read_query.cpp client/source/read_query.h client/source/session.cpp client/source/session.h client/source/query_reader.cpp client/source/query_reader.h client/source/answer_processor.cpp client/source/answer_processor.h)
target_link_libraries(client wsock32 ws2_32)

add_executable(test test.cpp server/source/server_sock.cpp server/source/server_sock.h
        client/source/client_sock.cpp client/source/client_sock.h
        socket.h socket.cpp query.cpp query.h answer.cpp answer.h protocol.cpp protocol.h)
target_link_libraries(test wsock32 ws2_32)


add_executable(test_server server/test/test.cpp server/test/test_database.h server/test/test_database.cpp server/source/database.h server/source/database.cpp)
