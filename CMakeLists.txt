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
)

target_compile_features(krompir_lib PUBLIC cxx_std_20)

find_package(fmt REQUIRED)
target_link_libraries(krompir_lib PRIVATE fmt::fmt)

find_package(spdlog REQUIRED)
target_link_libraries(krompir_lib PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

# ---- Declare executable ----

add_executable(krompir_exe src/main.cpp)
add_executable(krompir::exe ALIAS krompir_exe)

set_property(TARGET krompir_exe PROPERTY OUTPUT_NAME krompir)

target_compile_features(krompir_exe PRIVATE cxx_std_20)

target_link_libraries(krompir_exe PRIVATE krompir_lib)
target_link_libraries(krompir_exe PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)

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
