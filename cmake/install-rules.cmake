install(
    TARGETS krompir_exe
    RUNTIME COMPONENT krompir_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
