add_executable(mock_ptr_test)
target_sources(mock_ptr_test PRIVATE src/mock_ptr_test.cpp)
target_link_libraries(mock_ptr_test PRIVATE mock_ptr gmock_main)
