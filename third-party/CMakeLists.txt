cmake_minimum_required(VERSION 3.14)

# --- wxWidgets ---

block()
  # static linkage
  set(wxBUILD_SHARED OFF) 
  set(BUILD_SHARED_LIBS OFF) 

  # It seems wx wants these to default to being empty
  if(NOT DEFINED QT_PLATFORM_HDR)
    set(QT_PLATFORM_HDR "")
  endif()

  if(NOT DEFINED QT_PLATFORM_SRC)
    set(QT_PLATFORM_SRC "")
  endif()

  if(NOT DEFINED GTK_PLATFORM_HDR)
    set(GTK_PLATFORM_HDR "")
  endif()

  if(NOT DEFINED GTK_PLATFORM_SRC)
    set(GTK_PLATFORM_SRC "")
  endif()

  if(NOT DEFINED OPENGL_INCLUDE_DIR)
    set(OPENGL_INCLUDE_DIR "")
  endif()

  if(NOT DEFINED host_alias)
    set(host_alias "")
  endif()

  if(WIN32)
    set(wxTYPE_SA_HANDLER void) # Can be anything just need CMake to shut up
    set(wxTOOLKIT_DESC "")

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
