cmake_minimum_required(VERSION 3.10)
project(MyDiscord)

set(CMAKE_C_STANDARD 99)

include_directories(src/common)

add_executable(client
    src/client/client.c
    src/client/connector.c
    src/common/utils.c
)

add_executable(server
    src/server/server.c
    src/server/hoster.c
    src/common/utils.c
)

if(WIN32)
    target_link_libraries(client ws2_32 pthread)
    target_link_libraries(server ws2_32 pthread)
endif()
