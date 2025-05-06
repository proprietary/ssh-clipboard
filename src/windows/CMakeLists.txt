add_library(windows_clipboard_impl STATIC)
target_sources(windows_clipboard_impl
  PUBLIC_HEADER
  clipboard_impl.h
  PRIVATE
  clipboard_impl.cc
)
target_link_libraries(windows_clipboard_impl PRIVATE user32)

add_library(keyboard_emulator PRIVATE keyboard_emulator.cc PUBLIC keyboard_emulator.h)
target_link_libraries(keyboard_emulator PUBLIC user32)

add_executable(tray_paste WIN32 tray_paste.cc
target_link_libraries(tray_paste PRIVATE keyboard_emulator)
