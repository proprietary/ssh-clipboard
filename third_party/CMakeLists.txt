# Abseil
set(ABSL_PROPAGATE_CXX_STD ON)
add_subdirectory(abseil-cpp)

# GoogleTest
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
add_subdirectory(googletest)

# Google Benchmark
set(BENCHMARK_ENABLE_TESTING OFF)
add_subdirectory(benchmark)

# yaml-cpp
add_subdirectory(yaml-cpp)
