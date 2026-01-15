cmake_minimum_required(VERSION 3.20)
project(chatter C)

set(CMAKE_C_STANDARD 99)

add_library(monocypher
        src/external/monocypher.c
)
target_include_directories(monocypher PUBLIC inc)

add_library(chatlib
        src/libs.c
        src/tcp.c
)
target_include_directories(chatlib PUBLIC inc)

target_link_libraries(chatlib PUBLIC ws2_32)

add_executable(chat_server src/server.c
        src/shared.c)
target_link_libraries(chat_server PRIVATE chatlib monocypher)


add_executable(chat_client src/client.c
        src/shared.c)
target_link_libraries(chat_client PRIVATE chatlib monocypher)
