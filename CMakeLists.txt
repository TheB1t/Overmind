cmake_minimum_required(VERSION 3.12)
project(overmind)

include(FetchContent)

FetchContent_Declare(
    uuid_v4
    GIT_REPOSITORY https://github.com/crashoz/uuid_v4.git
    GIT_TAG master
)
FetchContent_MakeAvailable(uuid_v4)

include(cmake/definitions.cmake)

add_subdirectory(extension_common)
add_subdirectory(extension_server)
add_subdirectory(extension_client)