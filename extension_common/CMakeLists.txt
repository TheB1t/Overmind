project(overmind_common)

file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

target_compile_definitions(
    ${PROJECT_NAME} PUBLIC
    -DEXTENSION_VERSION_MAJOR=0x00
    -DEXTENSION_VERSION_MINOR=0x01
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    PREFIX ""
    SUFFIX ${OUTPUT_SUFFIX_STATIC}
    POSITION_INDEPENDENT_CODE ON
)

target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${PROJECT_NAME} PUBLIC uuid_v4::uuid_v4)
target_compile_options(${PROJECT_NAME} PRIVATE -mavx2)