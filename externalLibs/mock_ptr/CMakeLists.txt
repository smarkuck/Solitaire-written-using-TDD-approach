add_library(mock_ptr INTERFACE)
target_include_directories(mock_ptr INTERFACE headers)

add_subdirectory(unitTests)
