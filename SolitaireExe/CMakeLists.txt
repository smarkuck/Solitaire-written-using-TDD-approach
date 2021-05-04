add_executable(${PROJECT_NAME} WIN32)
target_include_directories(${PROJECT_NAME} PRIVATE headers)

target_sources(${PROJECT_NAME} PRIVATE
    sources/ApplicationFactory.cpp
    sources/main.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE SolitaireLib)
