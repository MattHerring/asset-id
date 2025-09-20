set(asset_id_TARGET_NAME asset_id)

set(asset_id_SRCS
  asset_id.cpp
  digit.cpp
  image_line.cpp
  write_png.cpp

  main.cpp
)

set(asset_id_INCLUDE
  "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>"
)

add_executable(${asset_id_TARGET_NAME} ${asset_id_SRCS})

set_target_properties(
  ${asset_id_TARGET_NAME} 
  PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CXX_EXTENSIONS ON
    INTERPROCEDURAL_OPTIMIZATION ON
    EXPORT_COMPILE_COMMANDS ON
)

target_include_directories(${asset_id_TARGET_NAME} PRIVATE ${asset_id_INCLUDE})

target_link_libraries(${asset_id_TARGET_NAME} PRIVATE -lpng -lz)

target_compile_options(
  ${asset_id_TARGET_NAME} 
  PUBLIC
    $<$<CONFIG:Release>:-Os;>
    $<$<CONFIG:Debug>:-Wall;-Werror;-Wextra;>
  PRIVATE
    -fvisibility=hidden
)
