cmake_minimum_required(VERSION 3.10)

project(Engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB SOURCES "src/*.cpp")


include_directories(
    include
    external/glad
    external/GLFW
)

add_library(glad STATIC external/glad/glad.c)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME}
    glad
    ${CMAKE_SOURCE_DIR}/external/glfw3.lib
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

set(RESOURCE_DIR ${CMAKE_SOURCE_DIR}/resources)
set(OUTPUT_DIR ${CMAKE_BINARY_DIR})

file(COPY ${RESOURCE_DIR} DESTINATION ${OUTPUT_DIR})