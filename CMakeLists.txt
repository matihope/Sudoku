cmake_minimum_required(VERSION 3.25)
project(SudokuByMatihope)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0054 NEW)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

# Custom target that is always executes.
# Creates a symlink to assets inside the build directory.
add_custom_target(create_asset_link ALL COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)

add_subdirectory(dependencies)
add_subdirectory(Engine)
add_subdirectory(src)
