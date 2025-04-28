cmake_minimum_required(VERSION 3.10)
project(MyDiscord)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

if(WIN32)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -lpthread")
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

include_directories(lib)

add_executable(server lib/server.c)
add_executable(client lib/client.c)

if(UNIX)
    target_link_libraries(server pthread)
    target_link_libraries(client pthread)
endif()

if(WIN32)
    set(CMAKE_EXECUTABLE_SUFFIX ".exe")
endif()