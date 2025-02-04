add_library(mac_clipboard_impl STATIC)
target_sources(mac_clipboard_impl
  PRIVATE
  mac_clipboard.h
  mac_clipboard.mm
  PUBLIC
  mac_clipboard.h
)
target_link_libraries(mac_clipboard_impl PRIVATE "-framework Cocoa")
target_include_directories(mac_clipboard_impl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(mac_clipboard_impl PROPERTIES COMPILE_FLAGS "-x objective-c++")


enable_testing()

add_executable(mac_clipboard_impl_test)
target_sources(mac_clipboard_impl_test PRIVATE mac_clipboard.test.cc)
target_link_libraries(mac_clipboard_impl_test PRIVATE mac_clipboard_impl gtest_main "-framework Cocoa")
set_target_properties(mac_clipboard_impl_test PROPERTIES COMPILE_FLAGS "-x objective-c++")
include(GoogleTest)
gtest_discover_tests(mac_clipboard_impl_test)
