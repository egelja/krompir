# Parent project does not export its library target, so this CML implicitly
# depends on being added from it, i.e. the testing is done only from the build
# tree and is not feasible from an install location

project(krompirTests LANGUAGES CXX)

# ---- Dependencies ----

find_package(Catch2 REQUIRED)
include(Catch)

# ---- Tests ----

add_executable(krompir_test source/krompir_test.cpp)
target_link_libraries(
    krompir_test PRIVATE
    krompir_lib
    Catch2::Catch2WithMain
)
target_compile_features(krompir_test PRIVATE cxx_std_20)

catch_discover_tests(krompir_test)

# ---- End-of-file commands ----

add_folders(Test)
