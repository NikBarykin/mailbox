cmake_minimum_required(VERSION 3.21)
project(mailbox)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_executable(server server/source/main.cpp server/source/server_sock.h
        general/source/socket.h general/source/socket.cpp server/source/server_sock.cpp server/source/database.cpp server/source/database.h general/source/query.cpp general/source/query.h general/source/answer.cpp general/source/answer.h server/source/server.cpp server/source/server.h general/source/protocol.cpp general/source/protocol.h server/test/test_database.cpp server/test/test_database.h server/source/query_processor.cpp server/source/query_processor.h server/test/test_query_processor.cpp server/test/test_query_processor.h server/test/test.cpp server/test/test_server.cpp server/test/test_server.h general/source/utils.cpp general/source/utils.h)
target_link_libraries(server wsock32 ws2_32)

add_executable(client client/source/main.cpp client/source/client_sock.h
        general/source/socket.h general/source/socket.cpp
        client/source/client_sock.cpp general/source/query.cpp general/source/query.h general/source/answer.cpp general/source/answer.h general/source/protocol.cpp general/source/protocol.h client/source/session.cpp client/source/session.h client/source/query_reader.cpp client/source/query_reader.h client/source/answer_processor.cpp client/source/answer_processor.h general/source/utils.cpp general/source/utils.h)
target_link_libraries(client wsock32 ws2_32)

add_executable(test general/test/test.cpp server/source/server_sock.cpp server/source/server_sock.h
        client/source/client_sock.cpp client/source/client_sock.h
        general/source/socket.h general/source/socket.cpp general/source/query.cpp general/source/query.h general/source/answer.cpp general/source/answer.h general/source/protocol.cpp general/source/protocol.h general/source/utils.cpp general/source/utils.h general/source/letter.cpp general/test/test_tools.cpp general/test/test_tools.h)
target_link_libraries(test wsock32 ws2_32)


add_executable(test_server server/test/test.cpp server/test/test_database.h server/test/test_database.cpp
        server/source/database.h server/source/database.cpp general/source/utils.cpp general/source/utils.h
        general/source/letter.h general/source/letter.cpp general/test/test_tools.cpp general/test/test_tools.h
        server/source/query_processor.h server/source/query_processor.cpp
        server/test/test_query_processor.h server/test/test_query_processor.cpp)
