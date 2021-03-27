add_executable(UnitTests)
target_sources(UnitTests PRIVATE piles/TableauPileTest.cpp)
target_link_libraries(UnitTests PRIVATE gmock_main)
