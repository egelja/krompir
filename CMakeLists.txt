cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    krompir
    VERSION 0.1.0
    DESCRIPTION "A Minecraft modpack creation toolkit."
    HOMEPAGE_URL "https://github.com/MrAwesomeRocks/krompir"
    LANGUAGES CXX
)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(DEBUG 1)
else()
  set(DEBUG 0)
endif()
configure_file(src/config.h.in config.h)
  
include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)

# ---- Declare library ----

add_library(
    krompir_lib OBJECT
    src/lib.cpp
    # Utilities
    src/logging.cpp
)

target_include_directories(
    krompir_lib ${warning_guard}
    PUBLIC
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/src>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/>"
)

target_compile_features(krompir_lib PUBLIC cxx_std_20)

find_package(fmt REQUIRED)
target_link_libraries(krompir_lib PRIVATE fmt::fmt)

find_package(spdlog REQUIRED)
target_link_libraries(krompir_lib PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

# ---- Declare ImGUI libs ----

find_package(imgui REQUIRED CONFIG)

add_library(
  imgui_bindings
  OBJECT
  third-party/bindings/imgui_impl_glfw.cpp
  third-party/bindings/imgui_impl_opengl3.cpp
)

target_include_directories(
  imgui_bindings ${warning_guard}
  PUBLIC
  "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/third-party/bindings>"
)
  
set_target_properties(
  imgui_bindings
  PROPERTIES
  CXX_CLANG_TIDY ""
  CXX_CPPCHECK ""
)

target_link_libraries(imgui_bindings PRIVATE imgui::imgui)

# ---- Declare executable ----

add_executable(krompir_exe
  src/main.cpp
  src/gui/gui.cpp
)
add_executable(krompir::exe ALIAS krompir_exe)

set_property(TARGET krompir_exe PROPERTY OUTPUT_NAME krompir)
target_compile_features(krompir_exe PRIVATE cxx_std_20)

target_link_libraries(krompir_exe PRIVATE krompir_lib)
target_link_libraries(krompir_exe PRIVATE imgui_bindings)

target_link_libraries(krompir_exe PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

find_package(argparse REQUIRED)
target_link_libraries(krompir_exe PRIVATE argparse::argparse)

# OpenGL/IMGUI stuff

# prefer newer OpenGL
# https://cmake.org/cmake/help/latest/policy/CMP0072.html
cmake_policy(SET CMP0072 NEW)

find_package(OpenGL REQUIRED)
find_package(GLEW CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

target_link_libraries(
  krompir_exe
  PRIVATE
  OpenGL::GL
  GLEW::GLEW
  glfw
)

target_compile_definitions(krompir_exe PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW)
target_link_libraries(krompir_exe PRIVATE imgui::imgui)

# ---- Install rules ----

if(NOT CMAKE_SKIP_INSTALL_RULES)
  include(cmake/install-rules.cmake)
endif()

# ---- Developer mode ----

if(NOT krompir_DEVELOPER_MODE)
  return()
elseif(NOT PROJECT_IS_TOP_LEVEL)
  message(
      AUTHOR_WARNING
      "Developer mode is intended for developers of krompir"
  )
endif()

include(cmake/dev-mode.cmake)
