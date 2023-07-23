cmake_minimum_required(VERSION 3.14)

# use GNU Patch from any platform
# https://github.com/scivision/cmake-patch-file/blob/main/cmake/PatchFile.cmake

if(WIN32)
  # prioritize Git Patch on Windows as other Patches may be very old and incompatible.
  find_package(Git)
  if(Git_FOUND)
    get_filename_component(GIT_DIR ${GIT_EXECUTABLE} DIRECTORY)
    get_filename_component(GIT_DIR ${GIT_DIR} DIRECTORY)
  endif()
endif()

find_program(PATCH
  NAMES patch
  HINTS ${GIT_DIR}
  PATH_SUFFIXES usr/bin
)

if(NOT PATCH)
  message(FATAL_ERROR "Did not find GNU Patch")
endif()

if(EXISTS "${in_file}.cmake_patched")
  message("The file ${in_file} has already been patched.")
else()
  if(DEFINED ${out_file})
    execute_process(COMMAND ${PATCH} ${in_file} --input=${patch_file} --output=${out_file} --ignore-whitespace
      TIMEOUT 15
      COMMAND_ECHO STDOUT
      RESULT_VARIABLE ret
    )
  else()
    execute_process(COMMAND ${PATCH} ${in_file} --input=${patch_file} --ignore-whitespace
      TIMEOUT 15
      COMMAND_ECHO STDOUT
      RESULT_VARIABLE ret
    )
  endif()

  if(NOT ret EQUAL 0)
    message(FATAL_ERROR "Failed to apply patch ${patch_file} to ${in_file} with ${PATCH}")
  else()
    file(TOUCH "${in_file}.cmake_patched")
  endif()
endif()
