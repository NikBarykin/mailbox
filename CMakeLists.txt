cmake_minimum_required(VERSION 3.21)
project(mailbox)

set(CMAKE_CXX_STANDARD 23)

include_directories(.)

add_executable(server server/source/main.cpp server/source/server_sock.h
        common/source/socket.h common/source/socket.cpp server/source/server_sock.cpp
        server/source/database.cpp server/source/database.h common/source/query.cpp common/source/query.h
        common/source/answer.cpp common/source/answer.h server/source/server.cpp server/source/server.h
        common/source/protocol.cpp common/source/protocol.h
        server/source/query_processor.cpp server/source/query_processor.h
        common/source/utils.cpp common/source/utils.h
        common/source/letter.h common/source/letter.cpp common/source/huffman_compression.cpp common/source/huffman_compression.h server/source/letter_filter.cpp server/source/letter_filter.h server/source/letter_filter_token.cpp server/source/letter_filter_token.h server/source/letter_filter_node.cpp server/source/letter_filter_node.h common/source/date.cpp common/source/date.h)
target_link_libraries(server wsock32 ws2_32)

add_executable(client client/source/main.cpp client/source/client_sock.h
        common/source/socket.h common/source/socket.cpp
        client/source/client_sock.cpp common/source/query.cpp common/source/query.h common/source/answer.cpp common/source/answer.h common/source/protocol.cpp common/source/protocol.h client/source/session.cpp client/source/session.h common/source/utils.cpp common/source/utils.h client/source/cycle_outcome_processing.cpp client/source/cycle_outcome_processing.h client/source/build_query.cpp client/source/build_query.h common/source/huffman_compression.cpp common/source/huffman_compression.h common/source/date.cpp common/source/date.h)
target_link_libraries(client wsock32 ws2_32)

add_executable(test_common common/test/main.cpp server/source/server_sock.cpp server/source/server_sock.h
        client/source/client_sock.cpp client/source/client_sock.h
        common/source/socket.h common/source/socket.cpp common/source/protocol.cpp common/source/protocol.h
        common/source/utils.cpp common/source/utils.h common/source/letter.cpp common/test/testing_utility.cpp
        common/test/testing_utility.h common/test/test_protocol.cpp common/test/test_protocol.h common/test/test_socket.cpp
        common/test/test_socket.h common/test/test_query.cpp common/test/test_query.h
        common/source/query.h common/source/query.cpp common/test/test_answer.cpp common/test/test_answer.h
        common/source/answer.h common/source/answer.cpp common/source/huffman_compression.cpp common/source/huffman_compression.h common/test/test_huffman_compression.cpp common/test/test_huffman_compression.h common/source/date.cpp common/source/date.h)
target_link_libraries(test_common wsock32 ws2_32)


add_executable(test_server server/test/main.cpp server/test/test_database.h server/test/test_database.cpp
        server/source/database.h server/source/database.cpp common/source/utils.cpp common/source/utils.h
        common/source/letter.h common/source/letter.cpp common/test/testing_utility.cpp common/test/testing_utility.h
        server/source/query_processor.h server/source/query_processor.cpp
        server/test/test_query_processor.h server/test/test_query_processor.cpp
        server/test/test_server.h server/test/test_server.cpp
        server/source/server.h server/source/server.cpp
        server/test/test_server.h server/test/test_server.cpp
        common/source/answer.h common/source/answer.cpp
        common/source/protocol.h common/source/protocol.cpp
        common/source/socket.h common/source/socket.cpp
        client/source/client_sock.h client/source/client_sock.cpp
        server/source/server_sock.h server/source/server_sock.cpp
        common/source/query.h common/source/query.cpp common/source/huffman_compression.cpp common/source/huffman_compression.h server/source/letter_filter.cpp server/source/letter_filter.h server/source/letter_filter_token.cpp server/source/letter_filter_token.h server/source/letter_filter_node.cpp server/source/letter_filter_node.h server/test/test_letter_filter.cpp server/test/test_letter_filter.h common/source/date.cpp common/source/date.h)
target_link_libraries(test_server wsock32 ws2_32)


add_executable(test_client client/test/main.cpp client/test/test_build_query.cpp client/test/test_build_query.h
        common/source/letter.h common/source/letter.cpp
        client/test/test_cycle_outcome_processing.cpp client/test/test_cycle_outcome_processing.h
        client/test/test_session.cpp client/test/test_session.h
        common/source/socket.h common/source/socket.cpp server/source/server_sock.h server/source/server_sock.cpp
        common/source/query.h common/source/query.cpp common/source/answer.h common/source/answer.cpp
        common/source/protocol.h common/source/protocol.cpp common/source/utils.h common/source/utils.cpp
        client/source/session.h client/source/session.cpp client/source/client_sock.h client/source/client_sock.cpp client/source/cycle_outcome_processing.cpp client/source/cycle_outcome_processing.h client/source/build_query.cpp client/source/build_query.h common/source/huffman_compression.cpp common/source/huffman_compression.h common/source/date.cpp common/source/date.h)
target_link_libraries(test_client wsock32 ws2_32)
