cmake_minimum_required(VERSION 3.16.3)

project(Solitaire)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/permissive-)
endif ()

add_library(SolitaireLib STATIC)
target_include_directories(SolitaireLib PUBLIC include)

target_sources(SolitaireLib PRIVATE
    src/cards/Card.cpp
    src/cards/ShuffledCardsGenerator.cpp
    src/piles/DefaultFoundationPile.cpp
    src/piles/DefaultStockPile.cpp
    src/piles/DefaultTableauPile.cpp
)

add_executable(${PROJECT_NAME} WIN32 src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)

add_subdirectory(libs)
add_subdirectory(ut)
