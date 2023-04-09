cmake_minimum_required(VERSION 3.14)

# --- wxWidgets ---

block()
  # static linkage
  set(wxBUILD_SHARED OFF) 

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
