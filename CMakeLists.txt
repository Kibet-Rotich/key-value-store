cmake_minimum_required(VERSION 3.16)

project(kv_store_system C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

find_package(Threads REQUIRED)

add_library(kv_store_lib STATIC
    kv_store.c
    parser.c
)

target_include_directories(kv_store_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(kv_server
    server.c
)

target_link_libraries(kv_server PRIVATE kv_store_lib)
target_link_libraries(kv_store_lib PRIVATE Threads::Threads)

target_compile_options(kv_store_lib PRIVATE -Wall -Wextra)
target_compile_options(kv_server PRIVATE -Wall -Wextra)
