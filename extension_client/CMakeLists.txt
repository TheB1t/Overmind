project(overmind_client)
include(FetchContent)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    PREFIX ""
    SUFFIX ${OUTPUT_SUFFIX_DYNAMIC}
    POSITION_INDEPENDENT_CODE ON
)

if(TARGET_ARCH STREQUAL "x86")
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}")
else()
    set_target_properties(${PROJECT_NAME} PROPERTIES OUTPUT_NAME "${PROJECT_NAME}_x64")
endif()

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    $<LINK_LIBRARY:WHOLE_ARCHIVE,overmind_common::overmind_common>
)