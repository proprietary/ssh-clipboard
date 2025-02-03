if(SSH_CLIPBOARD_BUILD_TESTS)
    # add_executable(unit_tests
    #     example_test.cpp
    # )
    
    # target_link_libraries(unit_tests
    #     PRIVATE
    #         ssh_clipboard
    #         GTest::gtest
    #         GTest::gtest_main
    #         GTest::gmock
    # )
    
    # add_test(NAME unit_tests COMMAND unit_tests)
endif()

if(SSH_CLIPBOARD_BUILD_BENCHMARKS)
    # add_executable(benchmarks
    #     my_benchmark.cpp
    # )
    
    # target_link_libraries(benchmarks
    #     PRIVATE
    #         ssh_clipboard
    #         benchmark::benchmark
    # )
endif()
