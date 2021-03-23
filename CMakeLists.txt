cmake_minimum_required(VERSION 3.16.3)

project(Solitaire)

if (MSVC)
    set(SDL2_DIR "${CMAKE_SOURCE_DIR}/SDL2-devel-2.0.14-VC")
    set(SDL2_INCLUDE_DIRS "${SDL2_DIR}/include")

    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(VC_LIB_PATH_SUFFIX lib/x64)
    else()
        set(VC_LIB_PATH_SUFFIX lib/x86)
    endif()

    set(SDL2_LIBRARIES_DIR "${SDL2_DIR}/${VC_LIB_PATH_SUFFIX}")
    set(SDL2_LIBRARY "${SDL2_LIBRARIES_DIR}/SDL2.lib")
    set(SDL2_MAIN_LIBRARY "${SDL2_LIBRARIES_DIR}/SDL2main.lib")
    set(SDL2_LIBRARIES ${SDL2_LIBRARY} ${SDL2_MAIN_LIBRARY})
else ()
    find_package(SDL2 REQUIRED)
endif ()

include_directories(${SDL2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} WIN32 src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})
