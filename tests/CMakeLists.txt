set(asset_id_test_TARGET_NAME asset_id_tests)

set(asset_id_test_SRCS
  ../src/asset_id.cpp
  ../src/digit.cpp
  ../src/image_line.cpp
  ../src/write_png.cpp

  asset_id_tests.cpp
  digit_tests.cpp
  image_line_tests.cpp
  write_png_tests.cpp
)

set(asset_id_test_INCLUDE
  #"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>"
  "$<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/external/Catch2/single_include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

add_executable(${asset_id_test_TARGET_NAME} ${asset_id_test_SRCS})

set_target_properties(${asset_id_test_TARGET_NAME} 
PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS ON
  INTERPROCEDURAL_OPTIMIZATION ON
  EXPORT_COMPILE_COMMANDS ON
)

target_include_directories(${asset_id_test_TARGET_NAME} PRIVATE ${asset_id_test_INCLUDE})
target_link_libraries(${asset_id_test_TARGET_NAME} PRIVATE Catch2::Catch2  -lpng -lz)

target_compile_options(${asset_id_test_TARGET_NAME} 
PUBLIC
  $<$<CONFIG:Release>:-Os;>
  $<$<CONFIG:Debug>:-Wall;-Werror;-Wextra;>
  PRIVATE
  -fvisibility=hidden
)

include(CTest)
include(Catch)

catch_discover_tests(${asset_id_test_TARGET_NAME})
