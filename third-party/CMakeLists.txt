cmake_minimum_required(VERSION 3.14)

block()
  # No linters
  set(CMAKE_CXX_CLANG_TIDY "")
  set(CMAKE_CXX_CPPCHECK "")

  # Extensions on
  set(CMAKE_CXX_EXTENSIONS ON)

  # Less CXX flags
  if(WIN32)
    if(MINGW)
      set(CMAKE_CXX_FLAGS
        "-D_FORTIFY_SOURCE=3 -fstack-protector-strong -fcf-protection=full"
      )
    else()
      set(CMAKE_CXX_FLAGS
        "/sdl                     \
        /guard:cf                 \
        /utf-8                    \
        /diagnostics:caret        \
        /W4                       \
        /permissive-              \
        /volatile:iso             \
        /Zc:inline                \
        /Zc:preprocessor          \
        /Zc:lambda                \
        /Zc:__cplusplus           \
        /Zc:externConstexpr       \
        /Zc:throwingNew           \
        /EHsc"
      )      
    endif()
  else() # *NIX
    set(CMAKE_CXX_FLAGS
      "-D_FORTIFY_SOURCE=3           \
       -fstack-protector-strong      \
       -fcf-protection=full          \
       -fstack-clash-protection"
    )
  endif()

  # --- wxWidgets ---
  
  # linkage
  set(wxBUILD_SHARED OFF)
  set(wxUSE_EXPAT "builtin")

  # turn this on since we have a modern MinGW
  set(wxUSE_DBGHELP ON)

  # Patch file
  execute_process(
    COMMAND ${CMAKE_COMMAND}
    -Din_file:FILEPATH=third-party/wxWidgets/build/cmake/build.cfg.in
    -Dpatch_file:FILEPATH=third-party/_patch/001-wxWidgets-undef-var.patch
    -P cmake/PatchFile.cmake
  )

  # Compile
  add_subdirectory(wxWidgets)
endblock()

block()
  set(BINLOG_BUILD_EXAMPLES OFF)
  set(BINLOG_BUILD_UNIT_TESTS OFF)
  set(BINLOG_BUILD_INTEGRATION_TESTS OFF)
  
  add_subdirectory(binlog)
endblock()
