cmake_minimum_required(VERSION 3.31)
project(ssh-clipboard VERSION 0.1.0 DESCRIPTION "Shared clipboard between host and SSH server" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(SSH_CLIPBOARD_BUILD_TESTS "Build test programs" ON)
option(SSH_CLIPBOARD_BUILD_BENCHMARKS "Build benchmark programs" ON)

add_subdirectory(third_party)

add_subdirectory(src)

if (SSH_CLIPBOARD_BUILD_TESTS OR SSH_CLIPBOARD_BUILD_BENCHMARKS)
  enable_testing()
  add_subdirectory(tests)
endif()
