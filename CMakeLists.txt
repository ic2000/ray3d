cmake_minimum_required(VERSION 3.12 FATAL_ERROR)
project(ray3d VERSION 1.0)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

add_compile_options(
  -Wall
  -Wextra
  -Wnon-virtual-dtor
  -pedantic
  -Wold-style-cast
  -Wcast-align
  -Wunused
  -Woverloaded-virtual
  -Wconversion
  -Wsign-conversion
  -Wmisleading-indentation
  -Wduplicated-cond
  -Wduplicated-branches
  -Wlogical-op
  -Wnull-dereference
  -Wuseless-cast
  -Wdouble-promotion
  -Wformat=2
  -Wimplicit-fallthrough
  -Wsuggest-attribute=const)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      Debug
      CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
endif()

include_directories(libs/rapidjson/include)
# add_subdirectory(libs/libzip)

find_package(
  SFML 2.5
  COMPONENTS system window graphics network audio
  REQUIRED)

include_directories(src/inc)

set(SRC
    src/util.cpp
    src/angle.cpp
    src/topdown_view.cpp
    src/game_view.cpp
    src/diagnostics_view.cpp
    src/raycasting.cpp
    src/player.cpp
    src/tilemap.cpp
    src/level.cpp
    src/engine.cpp
    src/main.cpp)

add_executable(${PROJECT_NAME} ${SRC})
target_link_libraries(${PROJECT_NAME} zip)
target_include_directories(${PROJECT_NAME} PRIVATE "${PROJECT_BINARY_DIR}")
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
target_link_libraries(${PROJECT_NAME} sfml-graphics)
install(TARGETS ${PROJECT_NAME} DESTINATION bin)

add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets
          ${CMAKE_CURRENT_BINARY_DIR}/assets)

if(NOT EXISTS ${PROJECT_BINARY_DIR}/.gitignore)
  file(WRITE ${PROJECT_BINARY_DIR}/.gitignore "*")
endif()
