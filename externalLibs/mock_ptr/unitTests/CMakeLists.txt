add_executable(mock_ptr_unitTests)

target_sources(mock_ptr_unitTests PRIVATE
    sources/mock_ptr_tests.cpp
    sources/mock_ptr_array_tests.cpp
)

target_link_libraries(mock_ptr_unitTests PRIVATE mock_ptr gmock_main)
