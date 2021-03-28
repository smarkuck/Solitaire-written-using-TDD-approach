cmake_minimum_required(VERSION 3.16.3)

project(Solitaire)

if (MSVC)
    add_compile_options(/permissive-)

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(VC_LIB_PATH_SUFFIX lib/x64)
    else()
        set(VC_LIB_PATH_SUFFIX lib/x86)
    endif()

    set(SDL2_DIR "${CMAKE_SOURCE_DIR}/libs/SDL2-devel-2.0.14-VC")
    set(SDL2_INCLUDE_DIRS "${SDL2_DIR}/include")
    set(SDL2_LIBRARIES_DIR "${SDL2_DIR}/${VC_LIB_PATH_SUFFIX}")
    set(SDL2_LIBRARY "${SDL2_LIBRARIES_DIR}/SDL2.lib")
    set(SDL2_MAIN_LIBRARY "${SDL2_LIBRARIES_DIR}/SDL2main.lib")
    set(SDL2_LIBRARIES ${SDL2_LIBRARY} ${SDL2_MAIN_LIBRARY})
else ()
    find_package(SDL2 REQUIRED)
endif ()

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${SDL2_INCLUDE_DIRS})

set(SOLITAIRE_LIB ${PROJECT_NAME}Lib)
add_library(${SOLITAIRE_LIB} STATIC)

target_sources(${SOLITAIRE_LIB} PRIVATE
    src/card/Card.cpp
    src/piles/TableauPile.cpp
)

add_executable(${PROJECT_NAME} WIN32 src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES})

add_subdirectory(libs)
add_subdirectory(ut)
