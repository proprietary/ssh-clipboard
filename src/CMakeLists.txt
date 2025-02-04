# Config

add_library(config STATIC)
target_compile_features(config INTERFACE cxx_std_23)
target_sources(config PRIVATE config.h config.cc)


# Core library

add_library(ssh_clipboard)

target_sources(ssh_clipboard
  PUBLIC
  ssh_clipboard.h
  PRIVATE
  ssh_clipboard.cc
  listener.h
  listener.cc
)

target_compile_features(ssh_clipboard INTERFACE cxx_std_23)


# Platform-specific clipboard access

if(WIN32)
  add_subdirectory(windows)
  target_link_libraries(ssh_clipboard windows_clipboard_impl)
elseif(APPLE)
  add_subdirectory(mac)
  target_link_libraries(ssh_clipboard
    PRIVATE
      config
      mac_clipboard_impl
    PUBLIC
      absl::base
      absl::strings
  )
elseif(UNIX)
  add_subdirectory(linux)
  target_link_libraries(ssh_clipboard linux_clipboard_impl)
else()
  message(FATAL_ERROR "Unsupported platform")
endif()

# Main executables

add_executable(ssh_clipboard_exe
  main.cc
)

target_compile_features(ssh_clipboard_exe INTERFACE cxx_std_23)

set_target_properties(ssh_clipboard_exe
  PROPERTIES
  OUTPUT_NAME "ssh_clipboard"
)

target_link_libraries(ssh_clipboard_exe
  PRIVATE
  ssh_clipboard
)
