cmake_minimum_required(VERSION 3.16.3)

project(Solitaire)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/permissive-)
endif ()

add_subdirectory(externalLibs)
add_subdirectory(SolitaireExe)
add_subdirectory(SolitaireLib)
