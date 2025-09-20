cmake_minimum_required(VERSION 3.24)
project(asset_id)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/Catch2/contrib")

add_subdirectory(external/Catch2)
add_subdirectory(src)
add_subdirectory(tests)