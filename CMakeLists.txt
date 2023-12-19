cmake_minimum_required(VERSION 3.14)

include(cmake/prelude.cmake)

project(
    krompir
    VERSION 0.1.0
    DESCRIPTION "A Minecraft modpack creation toolkit."
    HOMEPAGE_URL "https://github.com/MrAwesomeRocks/krompir"
    LANGUAGES CXX
)
  
include(cmake/project-is-top-level.cmake)
include(cmake/variables.cmake)
include(cmake/functions.cmake)

# ---- Enable compiler colored output ----

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fdiagnostics-color=always")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fdiagnostics-color=always")

# ---- Configuration File ----

configure_file(src/config.h.in config.h)

# ---- Load Deps ----

find_package(fmt REQUIRED)
find_package(argparse REQUIRED)

add_subdirectory(third-party)

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
    "$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/misc>"
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/>"
)

target_compile_features(krompir_lib PUBLIC cxx_std_20)

target_link_libraries(krompir_lib PRIVATE fmt::fmt)
target_link_libraries(krompir_lib PRIVATE binlog)

# ---- Declare executable ----

add_executable(krompir_exe
  src/main.cpp

  # Main GUI
  src/gui/gui.cpp
  src/gui/icons.cpp

  # Frames
  src/gui/frames/main.cpp
)
add_executable(krompir::exe ALIAS krompir_exe)

target_compile_features(krompir_exe PRIVATE cxx_std_20)
set_property(TARGET krompir_exe PROPERTY OUTPUT_NAME krompir)

# Libs
target_link_libraries(krompir_exe PRIVATE krompir_lib)

target_link_libraries(krompir_exe PRIVATE fmt::fmt)
target_link_libraries(krompir_exe PRIVATE binlog)

target_link_libraries(krompir_exe PRIVATE argparse::argparse)

target_link_libraries_system(
  krompir_exe
  PRIVATE
  wx::net wx::core wx::base
)

if(WIN32)
  target_sources(krompir_exe PRIVATE "misc/krompir.rc")
endif()

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
